

#include "riktelegram.hpp"
#include <phosphor-logging/log.hpp>

int main()
{
    boost::asio::io_service io;
    auto conn = std::make_shared<sdbusplus::asio::connection>(io);
    conn->request_name(RiktelegramServiceName);
    sdbusplus::asio::object_server server(conn);


    RiktelegramMgr riktelegramMgr(io, server, conn);

    io.run();
}
