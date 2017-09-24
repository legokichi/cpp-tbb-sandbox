#include <memory>
#include <iostream>
#include <string>
// http://www.boost.org/doc/libs/develop/doc/html/boost_asio/overview/core/handler_tracking.html
// need -lpthread
#define BOOST_ASIO_ENABLE_HANDLER_TRACKING
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#define BOOST_NETWORK_ENABLE_HTTPS 
#include <boost/network/protocol/http/client.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>

auto main(int argc, char* argv[])-> int {
  auto request = boost::network::http::client::request{"https://www.google.com/"};
  request << boost::network::header("Connection", "close");
  auto client = boost::network::http::client{};
  auto response = client.get(request);
  std::cout << boost::network::http::body(response) << std::endl;
  std::cout << boost::network::http::status(response) << std::endl;
}
