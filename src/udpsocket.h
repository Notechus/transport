//Sebastian Paulus 266446
#ifndef TRANSPORT_UDPSOCKET_H
#define TRANSPORT_UDPSOCKET_H

#include "utils.h"
#include "packet.h"
#include "packetbuffer.h"
#include <string>

class udpsocket {
private:
    std::string address;
    int port;
    int sock;
    sockaddr_in socketAddr;
    int packetFrame[PACKET_LIMIT];
    packetbuffer *buffer;

public:
    udpsocket(packetbuffer *buffer_, std::string address_, int port_);

    ~udpsocket();

    int run();

    ReceiverType getPacket(int start, int length);

    ssize_t sendPacket(int start, int length);

    std::string generateOutgoing(int start, int length);
};


#endif //TRANSPORT_UDPSOCKET_H
