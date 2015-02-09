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

#include "Receiver.h"
#include "FlightService.h"
#include "DatagramSocket.h"
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>

namespace org {
namespace hummingdroid {
namespace flightapp {

Receiver::Receiver(FlightService *context) :
    controller(&context->controller),
    telemetry(&context->telemetry),
    sensors(&context->sensors),
    motors(&context->motors),
    connected(false)
{
}

void Receiver::run()
{
    fprintf(stderr, "Receiver: Thread started\n");
    DatagramSocket socket;
    socket.bind(REMOTE_COMMAND_UDP_PORT);
    fcntl(socket.udp_socket, F_SETFL, O_NONBLOCK);

    while(true) {
        // Wait for incoming packet
        {
            struct pollfd fds = {socket.udp_socket, POLLIN, 0};
            poll(&fds, 1, connected ? REMOTE_COMMAND_TIMEOUT_MS : -1);
        }

        // Read the packet
        char data[2048];
        ssize_t size = read(socket.udp_socket, data, sizeof(data));
        if (size <= 0) {
            connected = false;
            fprintf(stderr, "Receiver: Link lost\n");
            continue;
        } else if (!connected) {
            fprintf(stderr, "Receiver: Link established\n");
            connected = true;
        }

        // Decode the packet
        CommandPacket command;
        if (!command.ParseFromArray(data, size)) {
            fprintf(stderr, "Received invalid CommandPacket\n");
            continue;
        }

        // Process the command
        if (command.has_command()) {
            controller->setCommand(command.command());
            telemetry->setCommand(command.command());
        }
        if (command.has_controller_config()) {
            controller->setConfig(command.controller_config());
        }
        if (command.has_telemetry_config()) {
            telemetry->setConfig(command.telemetry_config());
        }
        if (command.has_sensors_config()) {
            sensors->setConfig(command.sensors_config());
        }
        if (command.has_motors_config()) {
            motors->setConfig(command.motors_config());
        }
    }
}

}
}
}
