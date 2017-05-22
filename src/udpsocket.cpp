//Sebastian Paulus 266446
#include "udpsocket.h"
#include <algorithm>
#include <string.h>
#include <stdlib.h>

packet extractPacket(std::string data);

udpsocket::udpsocket(packetbuffer *buffer_, std::string address_, int port_, int upper_)
        : address(address_), port(port_) {
    this->buffer = buffer_;
    this->frameIdx = 0;
    this->upperBound = upper_;
}

udpsocket::~udpsocket() {
    close(sock);
}

int udpsocket::run() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cerr << "Error during socket creation\n";
        return -1;
    }

    bzero(&socketAddr, sizeof(socketAddr));
    inet_pton(AF_INET, address.c_str(), &socketAddr.sin_addr);
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons(port);

    return 0;
}

SocketStatus udpsocket::getPacket(int start, int length) {
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sock, &descriptors);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000;

    int received = select(sock + 1, &descriptors, NULL, NULL, &tv);

    if (received < 0) {
        std::cerr << "There was error while receiving data\n";
        return SocketStatus::Error;
    }
    if (received == 0) {
        return SocketStatus::NothingReceived;
    }

    char buff[2 * FRAME_SIZE];
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    int rec = (int) recvfrom(sock, buff, (2 * FRAME_SIZE), 0, (struct sockaddr *) &sender, &sender_len);
    if (rec < 0) {
        std::cerr << "There was error while receiving data\n";
        return SocketStatus::Error;
    }

    char senderAddr[16];
    int senderPort = ntohs(sender.sin_port);
    inet_ntop(AF_INET, &(sender.sin_addr), senderAddr, sizeof(senderAddr));
    std::string senderAddrS(senderAddr);
    if (senderAddrS == address && senderPort == port) {
        packet p = extractPacket(std::string(buff));
        if (p.getStatus() > 0 && p.getStart() >= start && p.getLength() == length) {
            if (p.getStart() == start && !packetFrame[frameIdx].received) {
                packetFrame[frameIdx].received = true;
                buffer->addPacket(p);

                return SocketStatus::MoveFrame;
            } else {
                int idx = (p.getStart() - start) / length;
                if (idx >= 0 && idx < upperBound && !packetFrame[idx].received) {
                    packetFrame[idx].received = true;
                    buffer->addPacket(p);
                }
                return SocketStatus::Normal;
            }
        }
    }

    return SocketStatus::Normal;
}

ssize_t udpsocket::sendPacket(int start, int length) {
    for (int i = 0; i < upperBound; i++) {
        int idx = (frameIdx + i) % upperBound;
        if (!packetFrame[idx].received || packetTimedOut(idx)) {
            std::string msg = generateOutgoing((start + i * length), length);
            unsigned long sent = (unsigned long) sendto(sock, msg.c_str(), strlen(msg.c_str()), 0,
                                                        (struct sockaddr *) &socketAddr,
                                                        sizeof(socketAddr));
            packetFrame[idx].time_sent = std::chrono::steady_clock::now();
            if (sent != msg.length()) {
                std::cerr << "There was error while sending packet: " + msg << std::endl;
                return -1;
            }
        }
    }
    return 1;
}

std::string udpsocket::generateOutgoing(int start, int length) {
    return "GET " + std::to_string(start) + " " + std::to_string(length) + "\n";
}

bool udpsocket::packetTimedOut(int idx) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - packetFrame[idx].time_sent);

    return elapsed.count() >= TIMEOUT;
}

int udpsocket::moveFrame() {
    int framesMoved = 0;
    while (packetFrame[frameIdx].received) {
        packetFrame[frameIdx].received = false;
        frameIdx = (frameIdx + 1) % upperBound;
        framesMoved++;
    }
    return framesMoved;
}

packet extractPacket(std::string data) {
    unsigned long first = data.find_first_of('\n');
    if (first > 0) {
        std::string dataHeader = data.substr(5, first - 5);
        std::string dataContent = data.substr(first + 1, data.length() - 1);
        unsigned long space = dataHeader.find_first_of(' ');
        if (space > 0) {
            try {
                int packetStart = std::stoi(dataHeader.substr(0, space));
                int packetLength = std::stoi(dataHeader.substr(space + 1, first));
                return packet(1, dataContent, packetStart, packetLength);
            } catch (...) {
                return packet(-1, "", 0, 0);
            }
        }
    }
    return packet(-1, "", 0, 0);
}
