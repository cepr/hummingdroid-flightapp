/*
 * HummingDroid, Android QuadCopter Controller
 * Copyright (C) 2013 Cedric Priscal
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Telemetry.h"
#include "Timestamp.h"
#include <stdio.h>
#include <unistd.h>

#define DEFAULT_PORT 49152

namespace org {
namespace hummingdroid {
namespace flightapp {

void Telemetry::setConfig(const CommandPacket::TelemetryConfig &config)
{
    synchronized
    bool new_host = config.has_host() && (!this->config.has_host() || this->config.host() != config.host());
    this->config = config;
    if (new_host) {
        // We want to avoid doing too many DNS requests
        socket.connect(config.host().c_str(),
                       config.has_port() ? config.port() : DEFAULT_PORT);
        notify();
    }
}

void Telemetry::setCommand(const Attitude & command)
{
    synchronized
    if (config.has_commandenabled() && config.commandenabled()) {
        packet.mutable_command()->CopyFrom(command);
        packet.mutable_command()->set_timestamp(Timestamp::now());
    }
}

void Telemetry::setAttitude(const Attitude & attitude)
{
    synchronized
    if (config.has_attitudeenabled() && config.attitudeenabled())
        packet.mutable_attitude()->CopyFrom(attitude);
}

void Telemetry::setControl(const MotorsControl & control)
{
    synchronized
    if (config.has_controlenabled() && config.controlenabled())
        packet.mutable_control()->CopyFrom(control);
}

void Telemetry::run()
{
    fprintf(stderr, "Telemetry: Thread started\n");

    while(true) {
        // Wait for the telemetry to be configured
        {
            synchronized
            while(!config.has_host()) {
                wait();
            }
        }
        fprintf(stderr,
                "Telemetry: Telemetry requested by %s\n",
                config.host().c_str());

        // Main connection loop
        while (true) {
            // Send a telemetry packet
            {
                synchronized
                std::string data = packet.SerializeAsString();
                if (socket.send(data.c_str(), data.length()) == -1) {
                    // The telemetry client is not here anymore, wait for another one to register
                    config.clear_host();
                    break;
                }
            }
            // Sleep
            usleep(50000);
        }
    }
}

}
}
}
