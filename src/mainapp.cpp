#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>

int main(int argc, char *argv[])
{
  if(argc < 2) {
    std::cerr << "Example 11: Wrong number of arguments" << std::endl 
	      << "Example 11: Usage: telegrambot msgfile" 
	      << std::endl;
    return EXIT_FAILURE;
  }
  
  char url[] = "https://api.telegram.org/bot5812037533:AAHYmDkoIbtYGBSFW7_-qmHu2hKtBV7YHlQ/sendMessage";//argv[1];
  char *msgfile = argv[1];
  FILE *f = fopen(msgfile,"rb");
  fseek(f,0,SEEK_END);
  long fsize = ftell(f);
  fseek(f,0,SEEK_SET);
  char *msg = (char *)malloc(fsize+29);
  strcpy(msg,"chat_id=-1001765034687&text=");
  fread(msg+28,fsize,1,f);
  msg[fsize+28]=0;
  fclose(f);
	
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

  free(msg);
  return EXIT_SUCCESS;
}
