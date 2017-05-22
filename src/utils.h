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

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define FRAME_SIZE 1000
#define TIMEOUT 1000
#define PACKET_LIMIT 1000

enum class SocketStatus {
    MoveFrame, Normal, Error, NothingReceived
};

struct packetack {
    std::chrono::steady_clock::time_point time_sent;
    bool received;

    packetack() {
        received = false;
        time_sent = std::chrono::steady_clock::now();
    }
};

#endif //TRANSPORT_UTILS_H
