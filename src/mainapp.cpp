#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <sstream>
#include <string>

#include <cerrno>

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>

int main(int argc, char *argv[])
{
  if(argc < 3) {
    std::cerr << "Example 11: Wrong number of arguments" << std::endl 
	      << "Example 11: Usage: telegrambot chat_id msgfile" 
	      << std::endl;
    return EXIT_FAILURE;
  }
  if (!strcmp(argv[1], "chatgpt"))
  {
	std::stringstream urls;
        urls << "https://api.openai.com/v1/completions -H \'Content-Type: application/json\' -H \'Accept-Encoding: gzip, deflate\' -H \'Authorization: Bearer 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\' -H \'User-Agent: AI%20CHAT/2 CFNetwork/1333.0.4 Darwin/21.5.0\' -H \'Accept-Language: en-EN,en;q=0.9\' -d \'{\"model\": \"text-davinci-003\",  \"max_tokens\": 3500, \"prompt\":" << argv[2] << "}\'";
	std::string url = urls.str();
	std::cout << std::endl << " URL: " << url << std::endl;
	  try {
		curlpp::Cleanup cleaner;
		curlpp::Easy request;

		using namespace curlpp::Options;
		request.setOpt(Verbose(true));
		request.setOpt(Url(url));

		request.perform();

		std::string effURL;
		curlpp::infos::EffectiveUrl::get(request, effURL);
		std::cout << "Effective URL: " << effURL << std::endl;

		//other way to retreive URL
		std::cout << std::endl 
			<< "Effective URL: " 
			<< curlpp::infos::EffectiveUrl::get(request)
			<< std::endl;

		std::cout << "Response code: " 
			<< curlpp::infos::ResponseCode::get(request) 
			<< std::endl;

		std::cout << "SSL engines: " 
			<< curlpp::infos::SslEngines::get(request)
			<< std::endl;
	  }
	  catch ( curlpp::LogicError & e ) {
	    std::cout << e.what() << std::endl;
	  }
	  catch ( curlpp::RuntimeError & e ) {
	    std::cout << e.what() << std::endl;
	  }
	return EXIT_SUCCESS;
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
