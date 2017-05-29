//Sebastian Paulus 266446
#include "packet.h"

packet::packet(int status_, std::vector<char> data_, int start_, int length_) :
        status(status_), start(start_), length(length_) {
    data = std::move(data_);
}

int packet::getStatus() const {
    return status;
}

const std::vector<char> &packet::getData() const {
    return data;
}

int packet::getLength() const {
    return length;
}

int packet::getStart() const {
    return start;
}
