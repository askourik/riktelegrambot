


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


#define CONFIG_FILE_NAME "/etc/rikmail/rikmail.conf"
#define MailFmt  "/usr/sbin/sendmail -t < /etc/rikmail/mail.txt"


void logMail(char *cstr)
{
        syslog(LOG_INFO, "Processing string %s",  cstr);
}
