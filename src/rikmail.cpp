/*
// Copyright (c) 2019 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#include "rikmail.hpp"
#include <phosphor-logging/log.hpp>
#include <phosphor-logging/elog-errors.hpp>
#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <vector>
#include <unordered_map>
#include <xyz/openbmc_project/Common/error.hpp>

#include <filesystem>

RikmailMgr::RikmailMgr(boost::asio::io_service& io_,
                     sdbusplus::asio::object_server& srv_,
                     std::shared_ptr<sdbusplus::asio::connection>& conn_) :
    io(io_),
    server(srv_), conn(conn_)
{
    iface = server.add_interface(RikmailPath, RikmailIface);
    //iface->register_method(
    //    "ReadMode", [this]() {
    //        return this->mode; 
    //    });
    //phosphor::logging::log<phosphor::logging::level::INFO>(
    //    ("Rikmail started mode " + mode).c_str());
    //iface->register_method(
    //    "WriteMode", [this](const std::string& mode) {
    //    });
    iface->initialize(true);

    int ret_code = 0;
    ret_code += system("systemctl start rikmail.service");
    if(ret_code)
        throw std::runtime_error("Errors occurred while setting timer");
    readConf();
    writeConf();

}

void RikmailMgr::readConf()
{
    system("/usr/sbin/newmail.sh");
    phosphor::logging::log<phosphor::logging::level::INFO>(
        "Rikmail readConf executed newmail.sh");
}


void RikmailMgr::writeConf()
{
    system("/usr/sbin/uptimer.sh");
    phosphor::logging::log<phosphor::logging::level::INFO>(
        "Rikmail writeConf executed uptimer.sh ");
}


