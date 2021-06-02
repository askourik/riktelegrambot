


#include <iostream>
#include <fstream>
#include <filesystem>
#include <exception>
#include <string>
#include <vector>

#include <cmath>
#include <cstdio>
#include <cstring>

#include <unistd.h>
#include <dirent.h>
#include <syslog.h>
#include <sys/types.h>
#include <sys/stat.h>


// #include <nlohmann/json.hpp>

namespace fs = std::filesystem;

// using json = nlohmann::json;
using namespace std::chrono_literals;


#define CONFIG_FILE_NAME "/usr/local/mail.conf"
#define custMailMode   (0)
#define minMailMode   (1)
#define nomMailMode   (2)
#define maxMailMode   (3)
#define MailFileFmt  "/usr/local/mailv"


struct MailDescr
{
    std::string from;
    std::string to;
    int period;
    int priority;
};

const std::vector<MailDescr> mailDescr
{
    {"aa@bb.cc", "aa@bb.cc", 2, 3}
};


 int mailmode_values[] = { 0, 1, 2, 3 };



/**
 * Для конфигурации одной для всех
 */
void sendPriority(unsigned int mailmode)
{
   // executeCmd("/sbin/sendmail", mailmode.c_str());
}

