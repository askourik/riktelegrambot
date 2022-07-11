
#include "rikmail.hpp"
#include <phosphor-logging/log.hpp>


int main()
{
    boost::asio::io_service io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name(RikmailServiceName);
    sdbusplus::asio::object_server server(conn);

    RikmailMgr rikmailMgr(io, server, conn);

    io.run();
}
