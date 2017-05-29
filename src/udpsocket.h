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
    packetack packetFrame[FRAME_SIZE];
    packetbuffer *buffer;
    int frameIdx;
    int upperBound = FRAME_SIZE;

public:
    udpsocket(packetbuffer *buffer_, std::string address_, int port_, int upper_);

    ~udpsocket();

    int run();

    SocketStatus getPacket(int start, int length);

    ssize_t sendPacket(int start, int length, int bound);

    std::string generateOutgoing(int start, int length);

    int moveFrame();
};


#endif //TRANSPORT_UDPSOCKET_H
