//Sebastian Paulus 266446
#include "udpsocket.h"

udpsocket::udpsocket(std::string address_, int port_)
        : address(address_), port(port_) {
}

udpsocket::~udpsocket() {
    close(sock);
}

int udpsocket::run() {
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        std::cerr << "Error during socket creation" << std::endl;
        return -1;
    }

    bzero(&socketAddr, sizeof(socketAddr));
    inet_pton(AF_INET, address.c_str(), &socketAddr.sin_addr);
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_port = htons(port);

    return 0;
}

packet udpsocket::getPacket() {
    fd_set descriptors;
    FD_ZERO (&descriptors);
    FD_SET (sock, &descriptors);
    struct timeval tv;
    tv.tv_sec = 2;
    tv.tv_usec = 0;

    int received = select(sock + 1, &descriptors, NULL, NULL, &tv);

    if (received < 0) {
        std::cerr << "There was error while receiving data" << std::endl;
        return packet(-1, 0);
    }
    if (received == 0) {
        return packet(0, 0);
    }

    char buff[2 * FRAME_SIZE];
    memset(buff, 0, 2 * FRAME_SIZE);
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);

    int rec = recvfrom(sock, buff, (2 * FRAME_SIZE), 0, (struct sockaddr *) &sender, &sender_len);

    if (rec < 0) {
        std::cerr << "There was error while receiving data" << std::endl;
        return packet(-1, 0);
    }

    char senderAddr[16];
    int senderPort = ntohs(sender.sin_port);
    inet_ntop(AF_INET, &(sender.sin_addr), senderAddr, sizeof(senderAddr));
    std::string senderAddrS(senderAddr);

    if (senderAddrS != address || senderPort != port) {
        return packet(0, 0);
    }

    return packet(1, buff);
}

ssize_t udpsocket::sendPacket(int start, int length) {
    std::string msg = generateOutgoing(start, length);
    unsigned long sent = sendto(sock, msg.c_str(), strlen(msg.c_str()), 0, (struct sockaddr *) &socketAddr,
                                sizeof(socketAddr));
    if (sent != msg.length()) {
        std::cerr << "There was error while sending packet: " + msg << std::endl;
        return -1;
    }

    return sent;
}

std::string udpsocket::generateOutgoing(int start, int length) {
    return "GET " + std::to_string(start) + " " + std::to_string(length) + "\n";
}
