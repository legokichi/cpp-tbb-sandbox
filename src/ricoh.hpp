#pragma once
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/algorithm/string.hpp>

auto encode64(const std::string &val) -> std::string {
  using namespace boost::archive::iterators;
  using encode = base64_from_binary<transform_width<std::string::const_iterator, 6, 8>>;
  auto tmp = std::string{encode(std::begin(val)), encode(std::end(val))};
  return tmp.append((3 - val.size() % 3) % 3, '=');
}

#pragma once
#include <cstdlib>
#include <string>
#include <memory>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/websocket.hpp>

namespace ricoh{
namespace sfu{

namespace asio = boost::asio;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;

template<class CompletionToken, class ReturnType>
using HandlerType = typename asio::handler_type<CompletionToken, void(boost::system::error_code, ReturnType)>::type;

template<class CompletionToken, class ReturnType>
using AsyncResult = typename asio::async_result<HandlerType<CompletionToken, ReturnType>>::type;


template<class CompletionToken>
auto wait(
  asio::io_service& ios,
  boost::posix_time::time_duration time,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, int> {
  using handler_type = HandlerType<CompletionToken, int>;
  auto handler = handler_type{std::forward<decltype(token)>(token)};
  auto result = asio::async_result<handler_type>{handler};
  boost::asio::spawn(ios, [&ios, time, handler](auto const yield) mutable {
    auto timer1 = boost::asio::deadline_timer{ios};
    timer1.expires_from_now(time);
    timer1.async_wait(yield);
    handler(boost::system::error_code{}, 0);
  });
  return result.get();
}


template<class CompletionToken>
auto httpRequest(
  std::shared_ptr<asio::io_service> ios,
  std::string host,
  http::request<http::string_body>&& req,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, http::response<http::dynamic_body>> {
  using handler_type = HandlerType<CompletionToken, http::response<http::dynamic_body>>;
  auto handler = handler_type{std::forward<decltype(token)>(token)};
  auto result = asio::async_result<handler_type>{handler};
  asio::spawn(*ios, [ios, host, req=std::move(req), handler=std::move(handler)](auto const yield) mutable {
    auto const query  = tcp::resolver::query{host, "https"};
    auto const lookup = tcp::resolver{*ios}.async_resolve(query, yield);
    auto ctx    = ssl::context{ssl::context::sslv23};
    auto socket = ssl::stream<tcp::socket>{*ios, ctx};
    asio::async_connect(socket.lowest_layer(), lookup, yield);
    socket.async_handshake(ssl::stream_base::client, yield);
    http::async_write(socket, req, yield);
    auto buffer = beast::flat_buffer{};
    auto res = http::response<http::dynamic_body>{};
    http::async_read(socket, buffer, res, yield);
    try{
      socket.async_shutdown(yield);
    }catch (std::exception& err){
    }
    handler(boost::system::error_code{}, std::move(res));
  });
  return result.get();
}


template<class CompletionToken>
auto getToken(
  std::shared_ptr<asio::io_service> ios,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, http::response<http::dynamic_body>> {
  using handler_type = HandlerType<CompletionToken, http::response<http::dynamic_body>>;
  auto handler = handler_type{std::forward<decltype(token)>(token)};
  auto result = asio::async_result<handler_type>{handler};
  namespace http = boost::beast::http;
  boost::asio::spawn(*ios, [ios, handler=std::move(handler)](auto const yield) mutable {
    auto host = "auth.api.ricoh";
    auto path = "/v1/token";
    auto req = http::request<http::string_body>{http::verb::post, path, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::content_type, "application/x-www-form-urlencoded; charset=utf-8");
    req.set(http::field::accept, "application/json");
    auto ACCESS_KEY = std::string{std::getenv("ACCESS_KEY")};
    auto SECRET_ACCESS_KEY = std::string{std::getenv("SECRET_ACCESS_KEY")};
    auto base64 = encode64(ACCESS_KEY + ":" + SECRET_ACCESS_KEY);
    req.set(http::field::authorization, "Basic " + base64);
    req.body() = "grant_type=client_credentials&scope=sfu.api.ricoh/v1/sfu";
    req.prepare_payload();
    auto res = httpRequest(ios, host, std::move(req), yield);
    handler(boost::system::error_code{}, std::move(res));
  });
  return result.get();
}

template<class CompletionToken>
auto callAPI(
  std::shared_ptr<boost::asio::io_service> ios,
  std::string accessToken,
  std::string path,
  std::string json,
  CompletionToken&& token
)-> typename boost::asio::async_result<typename boost::asio::handler_type<CompletionToken, void(boost::system::error_code, boost::beast::http::response<boost::beast::http::dynamic_body>)>::type>::type {
  using handler_type = typename boost::asio::handler_type<CompletionToken, void(boost::system::error_code, boost::beast::http::response<boost::beast::http::dynamic_body>)>::type;
  auto handler = handler_type{std::forward<decltype(token)>(token)};
  auto result = boost::asio::async_result<handler_type>{handler};
  namespace http = boost::beast::http;
  boost::asio::spawn(*ios, [ios, accessToken, path, json, handler=std::move(handler)](auto const yield) mutable {
    auto host = "sfu.api.ricoh";
    auto req = http::request<http::string_body>{http::verb::post, path, 11};
    req.set(http::field::host, host);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    req.set(http::field::authorization, "Bearer " + accessToken);
    req.set(http::field::accept, "application/json");
    req.set(http::field::content_type, "application/json");
    req.body() = json;
    req.prepare_payload();
    auto res = httpRequest(ios, host, std::move(req), yield);
    handler(boost::system::error_code{}, std::move(res));
  });
  return result.get();
}

template<class CompletionToken>
auto callAPI(
  std::shared_ptr<boost::asio::io_service> ios,
  std::string accessToken,
  std::string path,
  CompletionToken&& token
)-> typename boost::asio::async_result<typename boost::asio::handler_type<CompletionToken, void(boost::system::error_code, boost::beast::http::response<boost::beast::http::dynamic_body>)>::type>::type {
  return callAPI(ios, accessToken, path, "{}", std::forward(token));
}

template<class CompletionToken>
auto getRoomList(
  std::shared_ptr<boost::asio::io_service> ios,
  std::string accessToken,
  CompletionToken&& token
)-> typename boost::asio::async_result<typename boost::asio::handler_type<CompletionToken, void(boost::system::error_code, boost::beast::http::response<boost::beast::http::dynamic_body>)>::type>::type {
  return callAPI(ios, accessToken, "/v1/rooms", std::forward(token));
}



}
}
