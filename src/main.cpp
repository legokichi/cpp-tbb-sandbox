#include <memory>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#define BOOST_NETWORK_ENABLE_HTTPS 
#include <boost/network/protocol/http/client.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/version.hpp>

auto main(int argc, char* argv[])-> int {
  auto a = int{1};
  auto b = int{2};
  auto c = a + b;
  std::cout << "hello" << c << std::endl;
  return EXIT_SUCCESS;
}