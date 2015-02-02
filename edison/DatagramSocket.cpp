#include "DatagramSocket.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/udp.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

DatagramSocket::DatagramSocket()
{
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_socket == -1) {
        perror("Cannot create UDP packets.");
        exit(EXIT_FAILURE);
    }
}

DatagramSocket::~DatagramSocket()
{
    close(udp_socket);
}

void DatagramSocket::connect(const char *host, unsigned short port)
{
    synchronized

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    char port_decimal[32];
    sprintf(port_decimal, "%d", port);

    struct addrinfo *result = NULL;
    int s = getaddrinfo(host, port_decimal, &hints, &result);
    if (s) {
       fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
       exit(EXIT_FAILURE);
    }

    switch(result->ai_addr->sa_family) {
    case AF_INET:
        {
            union {
                uint32_t ip32;
                uint8_t ip8[4];
            } ip;
            ip.ip32 = ((struct sockaddr_in*)result->ai_addr)->sin_addr.s_addr;
            fprintf(stderr, "%s has been resolved to %d.%d.%d.%d\n", host, ip.ip8[0], ip.ip8[1], ip.ip8[2], ip.ip8[3]);
        }
        break;
    case AF_INET6:
        fprintf(stderr, "%s has been resolved to an IPV6 address\n", host);
        break;
    default:
        fprintf(stderr, "%s has been resolved to an unknown address familly\n", host);
        break;
    }

    if (::connect(udp_socket, result->ai_addr, result->ai_addrlen) == -1) {
        perror("DatagramSocket.cpp: connect() failed");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);
}

void DatagramSocket::bind(unsigned short port)
{
    synchronized

    const struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = htonl(INADDR_ANY)
        }
    };
    if (::bind(udp_socket, (const struct sockaddr*)&addr, sizeof(struct sockaddr_in)) == -1) {
        perror("DatagramSocket.cpp: bind() failed");
        exit(EXIT_FAILURE);
    }
}

void DatagramSocket::send(const void *data, int size)
{
    synchronized

    if (write(udp_socket, data, size) == -1) {
        perror("DatagramSocket: sendto() failed");
    }
}
