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
  if(argc < 3) {
    std::cerr << "Example 11: Wrong number of arguments" << std::endl 
	      << "Example 11: Usage: telegrambot chat_id msgfile" 
	      << std::endl;
    return EXIT_FAILURE;
  }
  
  char url[] = "https://api.telegram.org/bot5812037533:AAHYmDkoIbtYGBSFW7_-qmHu2hKtBV7YHlQ/sendMessage";
  char *chat_id = argv[1];
  char *msgfile = argv[2];
  FILE *f = fopen(msgfile,"rb");
  fseek(f,0,SEEK_END);
  long fsize = ftell(f);
  long msgsize = strlen(argv[1])+fsize+14;
  fseek(f,0,SEEK_SET);
  char *msg = (char *)malloc(msgsize+1);
  strcpy(msg,"chat_id=");
  strcat(msg,chat_id);
  strcat(msg,"&text=");
  fread(msg+msgsize-fsize,fsize,1,f);
  msg[msgsize]=0;
  fclose(f);
	
  try {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    
    request.setOpt(new curlpp::options::Url(url)); 
    request.setOpt(new curlpp::options::Verbose(true)); 
    
    request.setOpt(new curlpp::options::PostFields(msg));
    request.setOpt(new curlpp::options::PostFieldSize(fsize+29));
    
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
