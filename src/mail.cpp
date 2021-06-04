


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
#define minMail   (1)
#define nomMail   (2)
#define maxMail   (3)


struct MAILDescr
{
    std::string toname;
    std::string mailname;
    int mode;
    int priority;


};

const std::vector<MAILDescr> mailDescr
{
    {"User 1", "skourikhin@gmail.com", 1, 1},
    {"User 2", "skourikhin@gmail.com", 2, 2},
    {"User 3", "skourikhin@gmail.com", 3, 3},

};

#define MailFmt  "/usr/sbin/sendmail -t < /etc/rikmail/mail.txt"
#define MailsFmt  "/usr/sbin/sendmail -t < /etc/rikmail/mail%d.txt"


const int mailmode_values[] = {minMail, minMail, nomMail, maxMail};

/**
 * Для конфигурации одной для всех
 */
void sendMail(unsigned int mailmode)
{
    char cstr[256];
    std::fstream fd;

    if(mailmode >= (sizeof(mailmode_values) / sizeof(mailmode_values[0])))
    {
        syslog(LOG_ERR, "Wrong mailmode %d", mailmode);
        mailmode = (sizeof(mailmode_values) / sizeof(mailmode_values[0])) - 1;
    }

    auto readVal = mailmode_values[mailmode];
    
    // Изменение задания в ручном режиме
    for (const auto &it : mailDescr)
    {
        //ExecuteCmd(MailsFmt);
        std::sprintf(cstr, MailsFmt, it.mode);
        syslog(LOG_INFO, "Processing file %s mode %d",  cstr, mailmode);

        ////std::sprintf(cstr, MailsFmt, it.mode);
        ////fd.open(cstr, std::ios::out);

        // Установить значения ШИМ
        ////if (fd.is_open())
        ////{
        ////    fd << readVal;
        ////    if (!fd.good())
        ////    {
        ////        syslog(LOG_ERR, "Error %d write to file <%s>", errno, cstr);
        ////    }
        ////    fd.close();
        ////}
        ////else
        ////{
        ////    syslog(LOG_ERR, "Error %d open file <%s>", errno, cstr);
        ////}
    }

}
