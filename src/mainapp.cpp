/*
#include "riktelegram.hpp"
#include <phosphor-logging/log.hpp>

int main()
{
    boost::asio::io_service io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name(RiktelegramServiceName);
    sdbusplus::asio::object_server server(conn);


    RiktelegramMgr riktelegramMgr(io, server, conn);

    io.run();
}
*/
#include <cstdlib>
#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

int main(int argc, char *argv[])
{
  /*if(argc < 2) {
    std::cerr << "Example 11: Wrong number of arguments" << std::endl 
	      << "Example 11: Usage: example12 url" 
	      << std::endl;
    return EXIT_FAILURE;
  }*/
  
  char url[] = "https://api.telegram.org/bot5812037533:AAHYmDkoIbtYGBSFW7_-qmHu2hKtBV7YHlQ/sendMessage";//argv[1];
  
  try {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    
    request.setOpt(new curlpp::options::Url(url)); 
    request.setOpt(new curlpp::options::Verbose(true)); 
    
    //std::list<std::string> header; 
    //header.push_back("Content-Type: application/octet-stream"); 
    
    //request.setOpt(new curlpp::options::HttpHeader(header)); 
    
    request.setOpt(new curlpp::options::PostFields("chat_id=-1001765034687&text=Hello!Rikor!world"));
    request.setOpt(new curlpp::options::PostFieldSize(46));
    
    request.perform(); 
  }
  catch ( curlpp::LogicError & e ) {
    std::cout << e.what() << std::endl;
  }
  catch ( curlpp::RuntimeError & e ) {
    std::cout << e.what() << std::endl;
  }

  return EXIT_SUCCESS;
}
