#include "utils.h"
#include "udpsocket.h"

bool validateInput(int n, char **arguments);

int processPacket(packet p, int currentStart);

int validatePacket(packet p, int currentStart);

/*
 * 1. get 200 bytes
 * 2. ack 200 bytes
 * 3. write 200 bytes
 * 4. repeat
 */

int serverPort;
std::string outputFileName;
int fileLength;

int main(int argc, char **argv) {

    std::string serverAddr = "156.17.4.30";

    if (!validateInput(argc, argv)) {
        std::cerr << "Wrong arguments. You should run: ./transport PORT FILE_NAME FILE_LENGTH"
                  << std::endl;
    }
    int bytesLeft = fileLength;
    int currentStart = 0;
//    int packetQuant = std::ceil(((double) fileLength) / FRAME_SIZE);

    udpsocket s(serverAddr, serverPort);
    if (s.run() < 0) {
        std::cerr << "The application will close." << std::endl;
        return EXIT_FAILURE;
    }

    while (bytesLeft > 0) {
        int currentLength = bytesLeft >= FRAME_SIZE ? FRAME_SIZE : bytesLeft;
        int next = 0;
        while (!next) {
            int sent = s.sendPacket(currentStart, currentLength);
            if (sent < 0) {
                std::cerr << "The application will close." << std::endl;
                return EXIT_FAILURE;
            }

            packet p = s.getPacket();
            if (p.getStatus() < 0) {
                std::cerr << "The application will close." << std::endl;
                return EXIT_FAILURE;
            }

            if (processPacket(p, currentStart)) {
                bytesLeft -= FRAME_SIZE;
                currentStart += FRAME_SIZE;
                next = 1;
            }
        }
    }

    return 0;
}

int processPacket(packet p, int currentStart) {
    // response is DATA start length \n
    if (p.getStatus() > 0) {
        if (validatePacket(p, currentStart)) {
//        writeToFile(p);
            return 1;
        }
    }

    return 0;
}

int validatePacket(packet p, int currentStart) {
    std::string packetData = p.getDataAsString();
    unsigned long first = packetData.find_first_of('\n');
    if (first > 0) {
        std::string dataHeader = packetData.substr(5, first); // header minus 'DATA' word and space
//        std::string dataContent = packetData.substr(first + 1, packetData.length() - 1); // data here if valid
        unsigned long space = dataHeader.find_first_of(' ');
        if (space > 0) {
            int packetStart = std::stoi(dataHeader.substr(0, space));
            int packetLength = std::stoi(dataHeader.substr(space + 1, first));
            if (packetStart == currentStart && packetLength == FRAME_SIZE) return 1;
        }
    }

    return 0;
}


bool validateInput(int n, char **arguments) {
    if (n != 4) return false;

    serverPort = std::stoi(arguments[1]);
    outputFileName = arguments[2];
    fileLength = std::stoi(arguments[3]);

    if (fileLength < 0) return false;
    if (serverPort < 0 || serverPort > 65535) return false;

    return true;
}