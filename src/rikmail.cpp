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
#include "mail.h"

namespace fs = std::filesystem;


template <typename... ArgTypes>
static std::vector<std::string> executeCmd(const char* path,
                                           ArgTypes&&... tArgs)
{
    std::vector<std::string> stdOutput;
    boost::process::ipstream stdOutStream;
    boost::process::child execProg(path, const_cast<char*>(tArgs)...,
                                   boost::process::std_out > stdOutStream);
    std::string stdOutLine;

    while (stdOutStream && std::getline(stdOutStream, stdOutLine) &&
           !stdOutLine.empty())
    {
        stdOutput.emplace_back(stdOutLine);
    }

    execProg.wait();

    int retCode = execProg.exit_code();
    if (retCode)
    {
        phosphor::logging::log<phosphor::logging::level::ERR>(
            "Command execution failed",
            phosphor::logging::entry("PATH=%d", path),
            phosphor::logging::entry("RETURN_CODE:%d", retCode));
        phosphor::logging::elog<
            sdbusplus::xyz::openbmc_project::Common::Error::InternalFailure>();
    }

    return stdOutput;
}

RikmailMgr::RikmailMgr(boost::asio::io_service& io_,
                     sdbusplus::asio::object_server& srv_,
                     std::shared_ptr<sdbusplus::asio::connection>& conn_) :
    io(io_),
    server(srv_), conn(conn_)
{
    iface = server.add_interface(RikmailPath, RikmailIface);
    iface->register_method(
        "ReadMode", [this]() {
            phosphor::logging::log<phosphor::logging::level::INFO>(
                ("Rikmail register read mode " + this->mode).c_str());
            return this->mode; 
        });
    // iface->register_method("Read", [this](const std::string& key) {
    //     std::unordered_map<std::string, std::string> env = readAllVariable();
    //     auto it = env.find(key);
    //     if (it != env.end())
    //     {
    //         return it->second;
    //     }
    //     return std::string{};
    // });
    phosphor::logging::log<phosphor::logging::level::INFO>(
        ("Rikmail started mode " + mode).c_str());

    iface->register_method(
        "WriteMode", [this](const std::string& mode) {
            phosphor::logging::log<phosphor::logging::level::INFO>(
                ("Rikmail register write mode " + mode).c_str());
            setMailMode(mode);
        });
    iface->initialize(true);

    //executeCmd("/usr/sbin/mail.sh");
    phosphor::logging::log<phosphor::logging::level::INFO>(
        ("Rikmail executed mail.sh " + mode).c_str());


    int ret_code = 0;
//    ret_code += system("systemctl daemon-reload");
    ret_code += system("systemctl start rikmail.service");
    if(ret_code)
        throw std::runtime_error("Errors occurred while setting timer");

    this->mode = readConf();
    phosphor::logging::log<phosphor::logging::level::INFO>(
        ("Rikmail init mode " + mode).c_str());
    setMailMode(this->mode);

    //rikmail_set_timer("*-*-* *:00,02:00"); // once in 2 minutes
    //rikmail_set_timer("*-*-* *:00/05:00"); // once in 5 minutes

}

std::unordered_map<std::string, std::string> RikmailMgr::readAllVariable()
{
    std::unordered_map<std::string, std::string> env;
    std::vector<std::string> output = executeCmd("/sbin/fw_printenv");
    for (const auto& entry : output)
    {
        size_t pos = entry.find("=");
        if (pos + 1 >= entry.size())
        {
            phosphor::logging::log<phosphor::logging::level::ERR>(
                "Invalid U-Boot environment",
                phosphor::logging::entry("ENTRY=%s", entry.c_str()));
            continue;
        }
        // using string instead of string_view for null termination
        std::string key = entry.substr(0, pos);
        std::string value = entry.substr(pos + 1);
        env.emplace(key, value);
    }
    return env;
}

void RikmailMgr::setMailMode(const std::string& mode)
{
    phosphor::logging::log<phosphor::logging::level::INFO>(
        ("Rikmail set mode " + mode).c_str());

    try 
    {
       this->mode = mode;
    } 
    catch (const std::exception& e) 
    { 
         // std::cout << e.what();
        this->mode = "2_2_info@example.com";
    }
    //sendMail(this->mode.c_str());
    writeConf(this->mode);
    //executeCmd("/usr/sbin/mail.sh");
    return;
}


std::string RikmailMgr::readConf()
{
    std::string m = "2_2_info@example.com";
    fs::path conf_fname = "/etc/rikmail/rikmail.conf";
    try
    {
        std::ifstream conf_stream {conf_fname};
        conf_stream >> m;
        syslog(LOG_DEBUG, "rikmail read conf %s", m.c_str());
    }
    catch (const std::exception& e)
    {
        m = "2_2_info@example.com";
        syslog(LOG_DEBUG, "rikmail read conf exception %s", m.c_str());
        writeConf(m);
    }
    return m;
}


void RikmailMgr::writeConf(const std::string &m)
{
    syslog(LOG_DEBUG, "rikmail write conf %s size = %d", m.c_str(), m.size());
    fs::path conf_fname = "/etc/rikmail/rikmail.conf";
    {
        std::ofstream conf_stream {conf_fname};
        conf_stream << m;
    }
    int ret_code = 0;
    ret_code += system("/usr/sbin/uptimer.sh");
    phosphor::logging::log<phosphor::logging::level::INFO>(
        ("Rikmail executed uptimer.sh " + m).c_str());
    // ret_code += system("systemctl daemon-reload");
    // ret_code += system("systemctl restart rikmail.service");
    // if(ret_code)
    //     throw std::runtime_error("Errors occurred while setting timer");
}

//void RikmailMgr::rikmail_set_timer(const std::string &time_str)
//{
//    constexpr auto send_report_timer = "/lib/systemd/system/send-report.timer";
//    // constexpr auto dest = "org.freedesktop.systemd1";
//    // constexpr auto path = "/org/freedesktop/systemd1";
//    // constexpr auto interface = "org.freedesktop.systemd1.Manager";
//
//    std::string cmd = "sed -i 's/OnCalendar.*/OnCalendar=";
//    std::for_each(time_str.begin(), time_str.end(), [&cmd](const char c)
//            {
//                if(c == '*')
//                    cmd += "\\*";
//                else if (c == '/')
//                    cmd += "\\/";
//                else
//                    cmd += c;
//            });
//    cmd += "/g' ";
//    cmd += send_report_timer;
//    
//    syslog(LOG_DEBUG, "rikmail set timer %s", cmd.c_str());
//
//    int ret_code = 0;
//    ret_code += system(cmd.c_str());
//    ret_code += system("systemctl daemon-reload");
//    ret_code += system("systemctl restart send-report.timer");
//    if(ret_code)
//        throw std::runtime_error("Errors occurred while setting timer");
//}


