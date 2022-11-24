#pragma once

#include <boost/asio/io_service.hpp>
#include <sdbusplus/asio/object_server.hpp>

static constexpr const char* RiktelegramServiceName =
    "xyz.openbmc_project.rikmail";
static constexpr const char* RiktelegramIface =
    "xyz.openbmc_project.Rikmail";
static constexpr const char* RiktelegramPath =
    "/xyz/openbmc_project/rikmail";

class RiktelegramMgr
{
    enum class RiktelegramMode
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

    void setTelegramMode(const std::string& mode);
    std::string readConf();
    void writeConf(const std::string &m);

  public:
    RiktelegramMgr(boost::asio::io_service& io,
                sdbusplus::asio::object_server& srv,
                std::shared_ptr<sdbusplus::asio::connection>& conn);
};
