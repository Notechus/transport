//Sebastian Paulus 266446
#include "packet.h"

packet::packet(int status_, char *data_) :
        status(status_), data(data_) {

}

int packet::getStatus() {
    return status;
}

std::string packet::getDataAsString() {
    return std::string(data);
}

char *packet::getData() {
    return data;
}
