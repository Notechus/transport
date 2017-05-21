//Sebastian Paulus 266446
#ifndef TRANSPORT_PACKETBUFFER_H
#define TRANSPORT_PACKETBUFFER_H

#include "utils.h"
#include "packet.h"

#define BUFFER_SIZE 100

class packetbuffer {
private:
    std::map<int, packet> packets;
public:
    packetbuffer();

    void addPacket(packet p);

    packet findPacket(int start);
};


#endif //TRANSPORT_PACKETBUFFER_H
