//Sebastian Paulus 266446
#ifndef TRANSPORT_PACKET_H
#define TRANSPORT_PACKET_H


#include "utils.h"

class packet {
private:
    int status;
    char *data; // should be no more than FRAME_SIZE + '\0'

public:
    packet(int status_, char *data_);

    int getStatus();

    std::string getDataAsString();

    char *getData();
};


#endif //TRANSPORT_PACKET_H
