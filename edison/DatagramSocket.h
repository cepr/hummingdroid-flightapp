#ifndef DATAGRAMSOCKET_H
#define DATAGRAMSOCKET_H

#include "Object.h"

class DatagramSocket : public Object
{
public:
    DatagramSocket();
    ~DatagramSocket();
    void connect(const char *host, unsigned short port);
    void bind(unsigned short port);
    void send(const void *data, int size);
    int udp_socket;
};

#endif // DATAGRAMSOCKET_H
