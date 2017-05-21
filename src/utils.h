//Sebastian Paulus 266446
#ifndef TRANSPORT_UTILS_H
#define TRANSPORT_UTILS_H

#include <iostream>
#include <string>
#include <cmath>
#include <fstream>
#include <chrono>
#include <map>

#include <netinet/ip.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#define FRAME_SIZE 1000
#define TIMEOUT 2000
#define PACKET_LIMIT 20

enum class ReceiverType {
    CorrectPacket, IncorrectPacket, NothingReceived, Error
};

#endif //TRANSPORT_UTILS_H
