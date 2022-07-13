
#pragma once

#include <boost/asio/io_service.hpp>
#include <sdbusplus/asio/object_server.hpp>


static constexpr const char* RikmailServiceName =
    "xyz.openbmc_project.rikmail";
static constexpr const char* RikmailIface =
    "xyz.openbmc_project.Rikmail";
static constexpr const char* RikmailPath =
    "/xyz/openbmc_project/rikmail";

static const std::string initmode = "enable=false\n";

class RikmailMgr
{
    boost::asio::io_service& io;
    sdbusplus::asio::object_server& server;
    std::shared_ptr<sdbusplus::asio::connection> conn;
    std::shared_ptr<sdbusplus::asio::dbus_interface> iface;

    std::string mode = initmode;
    void setMailMode(const std::string& mode);
    std::string readConf();
    void writeConf(std::string mode);

  public:
    RikmailMgr(boost::asio::io_service& io,
                sdbusplus::asio::object_server& srv,
                std::shared_ptr<sdbusplus::asio::connection>& conn_);
};
