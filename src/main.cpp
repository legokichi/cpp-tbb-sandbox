#include <memory>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include "ricoh.hpp"
#include <boost/beast/http.hpp>
auto main(int argc, char* argv[])-> int {
  if(std::getenv("ACCESS_KEY") == nullptr || std::getenv("SECRET_ACCESS_KEY") == nullptr){
    std::cerr << "need ACCESS_KEY and SECRET_ACCESS_KEY env var" << std::endl;
    return EXIT_FAILURE;
  }
  auto ACCESS_KEY = std::string{std::getenv("ACCESS_KEY")};
  auto SECRET_ACCESS_KEY = std::string{std::getenv("SECRET_ACCESS_KEY")};
  auto ios = std::make_shared<boost::asio::io_service>();
  boost::asio::spawn(*ios, [&](auto yield) mutable {
    auto ec = boost::system::error_code{};
    //auto res = getToken(ios, ACCESS_KEY, SECRET_ACCESS_KEY, yield[ec]);
    auto host = "google.com";
    auto req = http::request<http::string_body>{http::verb::get, "/", 11};
    {auto ret = httpRequest(ios, host, req, yield[ec]);
      if(auto res_ptr = std::get_if<http::response<http::string_body>>(&ret)){
        std::cout << *res_ptr << std::endl;
      }else if(auto err_ptr = std::get_if<std::string>(&ret)){
        std::cout << *err_ptr << std::endl;
      }}
    {auto ret = httpsRequest(ios, host, req, yield[ec]);
    if(auto res_ptr = std::get_if<http::response<http::string_body>>(&ret)){
      std::cout << *res_ptr << std::endl;
    }else if(auto err_ptr = std::get_if<std::string>(&ret)){
      std::cout << *err_ptr << std::endl;
    }}
    std::cout << "hi" << std::endl;
    wait(ios, boost::posix_time::milliseconds(1000), yield[ec]);
    std::cout << "hi" << std::endl;
    wait(ios, boost::posix_time::milliseconds(1000), yield[ec]);
    std::cout << "hi" << std::endl;
  });
  ios->run();
  std::cout << "end" << std::endl;
  return EXIT_SUCCESS;
}
