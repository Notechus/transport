//Sebastian Paulus 266446
#ifndef TRANSPORT_UTILS_H
#define TRANSPORT_UTILS_H

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <chrono>
#include <map>
#include <algorithm>
#include <vector>

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define DATA_SIZE 1000
#define FRAME_SIZE 150

enum class SocketStatus {
    MoveFrame, Normal, Error, NothingReceived
};

struct packetack {
    bool received;

    packetack() {
        received = false;
    }
};

#endif //TRANSPORT_UTILS_H
