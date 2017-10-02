#include <memory>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include "ricoh.hpp"

auto main(int argc, char* argv[])-> int {
  auto ios = std::make_shared<boost::asio::io_service>();
  boost::asio::spawn(*ios, [ios](auto const yield) mutable {
    //auto res = ricoh::sfu::getToken(ios, yield);
    //std::cout << res << std::endl;
    std::cout << "hi" << std::endl;
    ricoh::sfu::wait(*ios, boost::posix_time::milliseconds(1000), yield);
    std::cout << "hi" << std::endl;
  });
  ios->run();
  std::cout << "end" << std::endl;
  return EXIT_SUCCESS;
}
