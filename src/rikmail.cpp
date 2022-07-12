
#include "rikmail.hpp"

#include <vector>
#include <unordered_map>
#include <filesystem>

#include <phosphor-logging/log.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>

#include <xyz/openbmc_project/Common/error.hpp>

namespace fs = std::filesystem;


RikmailMgr::RikmailMgr(boost::asio::io_service& io_,
                     sdbusplus::asio::object_server& srv_,
                     std::shared_ptr<sdbusplus::asio::connection>& conn_) :
    io(io_), server(srv_), conn(conn_)
{
    iface = server.add_interface(RikmailPath, RikmailIface);
    iface->register_method("ReadMode", [this]() { return this->mode; });

    iface->register_method(
        "WriteMode", [this](const std::string& mode) {
            this->setMailMode(mode);
        });

    iface->initialize(true);

    int ret_code = 0;
    ret_code += system("systemctl start rikmail.service");
    if(ret_code)
        throw std::runtime_error("Errors occurred while setting timer");
    this->mode = readConf();
    setMailMode(this->mode);
}


void RikmailMgr::setMailMode(const std::string& mode)
{
    phosphor::logging::log<phosphor::logging::level::INFO>(("Rikmail set mode " + mode).c_str());

    try 
    {
       this->mode = mode;
    } 
    catch (const std::exception& e) 
    { 
         // std::cout << e.what();
        this->mode = "0";
    }
    writeConf(this->mode);
    return;
}


std::string RikmailMgr::readConf()
{
    std::string  m = "2";
    fs::path conf_fname = "/etc/rikmail/mailnew.conf";
    try
    {
        std::ifstream conf_stream {conf_fname};
        conf_stream >> m;
        conf_stream.close();
    }
    catch (const std::exception& e)
    {
        m = "0";
        writeConf(m);
    }
    return m;
}


void RikmailMgr::writeConf(std::string m)
{
    fs::path conf_fname = "/etc/rikmail/mailnew.conf";
    std::ofstream conf_stream {conf_fname};
    conf_stream << m;
    conf_stream.close();
    if (m[0] != '0')
    {
    system("/usr/sbin/mailnew.sh");
    phosphor::logging::log<phosphor::logging::level::INFO>(
        "Rikmail writeConf executed mailnew.sh ");
    system("/usr/sbin/uptimer.sh");
    phosphor::logging::log<phosphor::logging::level::INFO>(
        "Rikmail writeConf executed uptimer.sh ");
    }
    else
    {
        phosphor::logging::log<phosphor::logging::level::INFO>(
            "Rikmail writeConf mailconf=0 ");
    }
}

