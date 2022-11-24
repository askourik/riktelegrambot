
#include "riktelegram.hpp"
#include <phosphor-logging/log.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <vector>
#include <unordered_map>
#include <xyz/openbmc_project/Common/error.hpp>

#include <filesystem>
#include "bot.h"

namespace fs = std::filesystem;

RiktelegramMgr::RiktelegramMgr(boost::asio::io_service& io_,
                     sdbusplus::asio::object_server& srv_,
                     std::shared_ptr<sdbusplus::asio::connection>& conn_) :
    io(io_),
    server(srv_), conn(conn_)
{
    iface = server.add_interface(RiktelegramPath, RiktelegramIface);
    iface->register_method(
        "ReadMode", [this]() {
            return this->mode; 
        });
    iface->register_method(
        "WriteMode", [this](const std::string& mode) {
            setTelegramMode(mode);
        });
    iface->initialize(true);

    int ret_code = 0;
    ret_code += system("systemctl start riktelegram.service");
    if(ret_code)
        throw std::runtime_error("Errors occurred while setting timer");

    this->mode = readConf();
    setTelegramMode(this->mode);
}

void RiktelegramMgr::setTelegramMode(const std::string& mode)
{
    try 
    {
       this->mode = mode;
    } 
    catch (const std::exception& e) 
    { 
        this->mode = "2_2_info@example.com";
    }
    writeConf(this->mode);
    return;
}


std::string RiktelegramMgr::readConf()
{
    std::string m = "2_2_info@example.com";
    fs::path conf_fname = "/etc/riktelegram/riktelegram.conf";
    try
    {
        std::ifstream conf_stream {conf_fname};
        conf_stream >> m;
    }
    catch (const std::exception& e)
    {
        m = "2_2_info@example.com";
        writeConf(m);
    }
    return m;
}


void RiktelegramMgr::writeConf(const std::string &m)
{
    fs::path conf_fname = "/etc/riktelegram/riktelegram.conf";
    {
        std::ofstream conf_stream {conf_fname};
        conf_stream << m;
    }
    int ret_code = 0;
    ret_code += system("/usr/sbin/riktelegrambot.sh");
}




