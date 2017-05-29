//Sebastian Paulus 266446
#ifndef TRANSPORT_PACKET_H
#define TRANSPORT_PACKET_H

#include "utils.h"

class packet {
private:
    int status;
    std::vector<char> data;
    int start;
    int length;


public:
    packet(int status_, std::vector<char> data_, int start_, int length_);

    int getStart() const;

    int getLength() const;

    int getStatus() const;

    const std::vector<char> &getData() const;
};


#endif //TRANSPORT_PACKET_H
