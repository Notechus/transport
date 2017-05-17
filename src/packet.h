//Sebastian Paulus 266446
#ifndef TRANSPORT_PACKET_H
#define TRANSPORT_PACKET_H


#include "utils.h"

class packet {
private:
    int start;
    int length;
    int status;
    std::string data;

public:
    packet(int status_, std::string data_, int start_, int length_);

    int getStart() const;

    int getLength() const;

    int getStatus() const;

    std::string getData() const;
};


#endif //TRANSPORT_PACKET_H
