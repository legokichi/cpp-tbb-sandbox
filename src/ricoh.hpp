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
#include <optional>
#include <variant>
#include <utility>
#include <tuple>
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

using std::string;
using std::pair;
using std::tuple;
using std::variant;
using std::optional;
using std::shared_ptr;

namespace asio = boost::asio;
namespace beast = boost::beast;
using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;
namespace http = boost::beast::http;
using boost::system::error_code;

template<class CompletionToken, class ReturnType>
using HandlerType = typename asio::handler_type<CompletionToken, void(ReturnType)>::type;

template<class CompletionToken, class ReturnType>
using AsyncResult = typename asio::async_result<HandlerType<CompletionToken, ReturnType>>::type;


template<class CompletionToken>
auto wait(
  const shared_ptr<asio::io_service> ios,
  const boost::posix_time::time_duration time,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, error_code> {
  using handler_t = HandlerType<CompletionToken, error_code>;
  auto handler = handler_t{std::forward<CompletionToken>(token)};
  auto result = asio::async_result<handler_t>{handler};
  asio::spawn(*ios, [&, handler](auto yield) mutable {
    auto ec = error_code{};
    auto timer = asio::deadline_timer{*ios};
    timer.expires_from_now(time);
    timer.async_wait(yield[ec]);
    handler(std::move(ec));
  });
  return result.get();
}

template<class CompletionToken>
auto httpRequest(
  const shared_ptr<asio::io_service> ios,
  const string& host,
  const http::request<http::string_body>& req,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, variant<string, http::response<http::string_body>>> {
  using ret_t = variant<string, http::response<http::string_body>>;
  using handler_t = HandlerType<CompletionToken, ret_t>;
  auto handler = handler_t{std::forward<CompletionToken>(token)};
  auto result = asio::async_result<handler_t>{handler};
  asio::spawn(*ios, [&, handler](auto yield) mutable {
    auto ec = error_code{};
    auto query  = tcp::resolver::query{host, "http"};
    auto lookup = tcp::resolver{*ios}.async_resolve(query, yield[ec]);
    if(ec != 0){ return handler(ec, ret_t{"lookup error"}); }
    auto socket = tcp::socket{*ios};
    asio::async_connect(socket, lookup, yield[ec]);
    if(ec != 0){ return handler(ret_t{"connect error"}); }
    http::async_write(socket, const_cast<http::request<http::string_body>&>(req), yield[ec]);
    if(ec != 0){ return handler(ret_t{"write error"}); }
    auto buffer = beast::flat_buffer{};
    auto res = http::response<http::string_body>{};
    http::async_read(socket, buffer, res, yield[ec]);
    if(ec != 0){ return handler(ret_t{"read error"}); }
    socket.shutdown(tcp::socket::shutdown_both, ec);
    if(ec != 0){ return handler(ret_t{"shutdown error"}); }
    handler(ret_t{std::move(res)});
  });
  return result.get();
}

template<class CompletionToken>
auto httpsRequest(
  const shared_ptr<asio::io_service> ios,
  const string& host,
  const http::request<http::string_body>& req,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, variant<string, http::response<http::string_body>>> {
  using ret_t = variant<string, http::response<http::string_body>>;
  using handler_t = HandlerType<CompletionToken, ret_t>;
  auto handler = handler_t{std::forward<CompletionToken>(token)};
  auto result = asio::async_result<handler_t>{handler};
  asio::spawn(*ios, [&, handler](auto yield) mutable {
    auto ec = error_code{};
    auto lookup = tcp::resolver{*ios}.async_resolve(tcp::resolver::query{host, "https"}, yield[ec]);
    if(ec != 0){ return handler(ec, ret_t{"lookup error"}); }
    auto ctx    = ssl::context{ssl::context::sslv23};
    auto ssl_socket = ssl::stream<tcp::socket>{*ios, ctx};
    asio::async_connect(ssl_socket.lowest_layer(), lookup, yield[ec]);
    if(ec != 0){ return handler(ret_t{"connect error"}); }
    ssl_socket.async_handshake(ssl::stream_base::client, yield[ec]);
    if(ec != 0){ return handler(ret_t{"handshake error"}); }
    http::async_write(ssl_socket, const_cast<http::request<http::string_body>&>(req), yield[ec]);
    if(ec != 0){ return handler(ret_t{"write error"}); }
    auto buffer = beast::flat_buffer{};
    auto res = http::response<http::string_body>{};
    http::async_read(ssl_socket, buffer, res, yield[ec]);
    if(ec != 0){ return handler(ret_t{"read error"}); }
    ssl_socket.lowest_layer().cancel();
    ssl_socket.async_shutdown(yield[ec]);
    ssl_socket.lowest_layer().close();
    std::cout << "ha??" << std::endl;
    if(ec != 0){ return handler(ret_t{"shutdown error"}); }
    // https://github.com/boostorg/beast/issues/38
    // segmentation fault
    handler(ret_t{std::move(res)});
  });
  return result.get();
}


template<class CompletionToken>
auto getToken(
  const shared_ptr<asio::io_service> ios,
  const string& ACCESS_KEY,
  const string& SECRET_ACCESS_KEY,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, variant<string, http::response<http::string_body>>> {
  using ret_t = variant<string, http::response<http::string_body>>;
  using handler_type = HandlerType<CompletionToken, ret_t>;
  auto handler = handler_type{std::forward<CompletionToken>(token)};
  auto result = asio::async_result<handler_type>{handler};
  asio::spawn(*ios, [&, handler](auto yield) mutable {
    auto host = "auth.api.ricoh";
    auto path = "/v1/token";
    auto req = http::request<http::string_body>{http::verb::post, path, 11};
    req.set(http::field::accept, "application/json");
    req.set(http::field::authorization, "Basic " + encode64(ACCESS_KEY + ":" + SECRET_ACCESS_KEY));
    req.set(http::field::content_type, "application/x-www-form-urlencoded; charset=utf-8");
    req.body() = "grant_type=client_credentials&scope=sfu.api.ricoh/v1/sfu";
    req.prepare_payload();
    auto ret = httpRequest(ios, host, req, yield);
    if(auto err_ptr = std::get_if<string>(&ret)){
      return handler(ret_t{*err_ptr});
    }else if(auto res_ptr = std::get_if<http::response<http::string_body>>(&ret)){
      return handler(ret_t{*res_ptr});
    }
    return handler(ret_t{"unknown type error"});
  });
  return result.get();
}

template<class CompletionToken>
auto callAPI(
  const shared_ptr<asio::io_service> ios,
  const string& accessToken,
  const string& path,
  const string& json,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, variant<string, http::response<http::string_body>>> {
  using ret_t = variant<string, http::response<http::string_body>>;
  using handler_type = HandlerType<CompletionToken, ret_t>;
  auto handler = handler_type{std::forward<CompletionToken>(token)};
  auto result = asio::async_result<handler_type>{handler};
  asio::spawn(*ios, [&, handler](auto yield) mutable {
    auto host = "sfu.api.ricoh";
    auto req = http::request<http::string_body>{http::verb::post, path, 11};
    req.set(http::field::authorization, "Bearer " + accessToken);
    req.set(http::field::accept, "application/json");
    req.set(http::field::content_type, "application/json");
    req.body() = json;
    req.prepare_payload();
    auto ret = httpRequest(ios, host, req, yield);
    if(auto err_ptr = std::get_if<string>(&ret)){
      return handler(ret_t{*err_ptr});
    }else if(auto res_ptr = std::get_if<http::response<http::string_body>>(&ret)){
      return handler(ret_t{*res_ptr});
    }
    return handler(ret_t{"unknown type error"});
  });
  return result.get();
}

template<class CompletionToken>
auto callAPI(
  const shared_ptr<asio::io_service> ios,
  const string& accessToken,
  const string& path,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, variant<string, http::response<http::string_body>>> {
  return callAPI(ios, accessToken, path, "{}", std::forward(token));
}

template<class CompletionToken>
auto getRoomList(
  const shared_ptr<asio::io_service> ios,
  std::string accessToken,
  CompletionToken&& token
)-> AsyncResult<CompletionToken, variant<string, http::response<http::string_body>>> {
  return callAPI(ios, accessToken, "/v1/rooms", std::forward(token));
}

