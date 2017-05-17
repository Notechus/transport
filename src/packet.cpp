//Sebastian Paulus 266446
#include "packet.h"

packet::packet(int status_, std::string data_, int start_, int length_) :
        status(status_), data(data_), start(start_), length(length_) {

}

int packet::getStatus() const {
    return status;
}

std::string packet::getData() const {
    return data;
}

int packet::getLength() const {
    return length;
}

int packet::getStart() const {
    return start;
}
