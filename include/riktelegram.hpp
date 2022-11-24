#pragma once

#include <boost/asio/io_service.hpp>
#include <sdbusplus/asio/object_server.hpp>

static constexpr const char* RikmailServiceName =
    "xyz.openbmc_project.rikmail";
static constexpr const char* RikmailIface =
    "xyz.openbmc_project.Rikmail";
static constexpr const char* RikmailPath =
    "/xyz/openbmc_project/rikmail";

class RikmailMgr
{
    enum class RikmailMode
    {
        AUTO = 0,
        MINIMAL = 1,
        OPTIMAL = 2,
        MAXIMAL = 3
    };

    boost::asio::io_service& io;
    sdbusplus::asio::object_server& server;
    std::shared_ptr<sdbusplus::asio::connection> conn;
    std::shared_ptr<sdbusplus::asio::dbus_interface> iface;

    std::string mode = "2_2_info@example.com";

    std::unordered_map<std::string, std::string> readAllVariable();
    void setMailMode(const std::string& mode);
    std::string readConf();
    void writeConf(const std::string &m);
    //void rikmail_set_timer(const std::string &time_str);

  public:
    RikmailMgr(boost::asio::io_service& io,
                sdbusplus::asio::object_server& srv,
                std::shared_ptr<sdbusplus::asio::connection>& conn);
};
