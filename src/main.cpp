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
  return EXIT_SUCCESS;
}