//Sebastian Paulus 266446
#include "packetbuffer.h"

packetbuffer::packetbuffer() {
}

void packetbuffer::addPacket(packet p) {
    if (packets.size() >= BUFFER_SIZE) packets.clear();
    packets.insert(std::pair<int, packet>(p.getStart(), p));
}

packet packetbuffer::findPacket(int start) {
    auto found = packets.find(start);

    if (found != packets.end()) {
        return found->second;
    } else {
        return packet(-1, "", 0, 0);
    }
}

bool packetbuffer::nextAvailable(int number) {
    auto found = packets.find(number);

    return found != packets.end();
}
