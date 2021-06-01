


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
    {"localhost@rikor-scalable.local", "skourikhin@gmail.com", 60, 1}
};


 std::string mailmode_values[] = {"custMailMode", "min_MailMode", "nom_MailMode", "max_MailMode"};


#if 0
int rawPWM(std::string perc)
{
    int readVal;
    try
    {
        readVal = std::lround(std::stoi(perc) * 255.0 / 100.0);
    }
    catch (const std::exception &e)
    {
        readVal = nomPWMraw;
    }
    return readVal;
}

std::string percPWM(int raw)
{
    if (raw < 0)
        return std::to_string(raw);
    return std::to_string(std::lround(raw * 100.0 / 255.0));
}


std::string percFAN(int raw)
{
    return std::to_string(raw);
}

int fillState(json &jdata)
{
    std::ifstream ifd;
    int readVal;
    json pFT;
    char fname[256];

    jdata["fantach"] = json::array();
    for (const auto &it : fanDescr)
    {
        pFT = json::array();
        pFT += it.name;

        readVal = 0;
        std::sprintf(fname, FanFileFmt, it.tach);
        ifd.open(fname);
        if (ifd.is_open())
        {
            ifd >> readVal;
            if (!ifd.good())
                readVal = -1;
            ifd.close();
        }
        else
        {
            readVal = -2;
        }
        pFT += percFAN(readVal);

        readVal = 0;
        std::sprintf(fname, PWMFileFmt, it.pwm);
        ifd.open(fname);
        if (ifd.is_open())
        {
            ifd >> readVal;
            if (!ifd.good())
                readVal = -1;
            ifd.close();
        }
        pFT += percPWM(readVal);

        jdata["fantach"].emplace_back(pFT);
    }
    return 0;
}



/**
 * Для именованых каналов
 */
void setFansForList()
{
    int num;
    json newPwm;
    // Изменение задания в ручном режиме
    for (const auto &it : fanDescr)
    {
        // Чтение текущих значений PWM
        std::sprintf(cstr, PWMFileFmt, it.pwm);
        fd.open(cstr, std::ios::in);
        if (fd.is_open())
        {
            fd >> readVal;
            if (!fd.good())
                readVal = nomPWMraw;
            fd.close();
        }
        else
        {
            readVal = nomPWMraw;
        }

        if (jin.count(it.webname) > 0)
        {
            readVal = rawPWM(jin[it.webname].get<std::string>());
        }

        if (readVal < minPWMraw)
            readVal = minPWMraw;

        std::sprintf(cstr, PWMFileFmt, it.pwm);
        fd.open(cstr, std::ios::out);

        // Установить значения ШИМ
        if (fd.is_open())
        {
            fd << readVal;
            if (!fd.good())
            {
                syslog(LOG_ERR, "Error %d write to file <%s>", errno, cstr);
            }
            fd.close();
        }
        else
        {
            syslog(LOG_ERR, "Error %d open file <%s>", errno, cstr);
        }

        newPwm["pwm" + std::to_string(it.pwm)] = readVal;
    } // for

    // Сохранить в файле конфигурации новые значения.
    // Они будут установлены при перезагрузке.
    std::ofstream confout {CONFIG_FILE_NAME};
    confout << newPwm;
    confout.close();
}
#endif

/**
 * Для конфигурации одной для всех
 */
void setMailMode(unsigned int mailmode)
{
    char cstr[256];
    std::fstream fd;

    if(mailmode >= 4)
    {
        syslog(LOG_ERR, "Wrong mailmode %d", mailmode);
        mailmode = custMailMode;
    }

    auto readVal = mailmode_values[mailmode];
    
    // Изменение задания в ручном режиме
    for (const auto &it : mailDescr)
    {
        std::sprintf(cstr, mailFileFmt, it.priority);
        fd.open(cstr, std::ios::out);

        // Установить значения priority
        if (fd.is_open())
        {
            fd << readVal;
            if (!fd.good())
            {
                syslog(LOG_ERR, "Error %d write to file <%s>", errno, cstr);
            }
            fd.close();
        }
        else
        {
            syslog(LOG_ERR, "Error %d open file <%s>", errno, cstr);
        }
    }
}

