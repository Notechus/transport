//Sebastian Paulus 266446
#include "utils.h"
#include "udpsocket.h"

bool validateInput(int n, char **arguments);

int processPacket(const packet &p, int currentStart, int currentLength, std::ofstream &file);

void writeToFile(const packet &p, std::ofstream &file);

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
    bool next;

    std::ofstream output;
    output.open(outputFileName);
    packetbuffer *buff = new packetbuffer();
    int minimum = std::min(FRAME_SIZE, (fileLength / DATA_SIZE));
    udpsocket s(buff, serverAddr, serverPort, minimum);

    if (s.run() < 0) {
        std::cerr << "The application will close." << std::endl;
        output.close();
        delete buff;
        return EXIT_FAILURE;
    }

    std::cout << "Receiving data.\n";

    std::chrono::steady_clock::time_point start, end;

    while (bytesLeft > 0) {
        int currentLength = bytesLeft >= DATA_SIZE ? DATA_SIZE : bytesLeft;
        next = false;
        int sent = (int) s.sendPacket(currentStart, currentLength, (bytesLeft / currentLength));
        if (sent < 0) {
            std::cerr << "The application will close." << std::endl;
            output.close();
            delete buff;
            return EXIT_FAILURE;
        }

        while (!next) {
            auto received = s.getPacket(currentStart, currentLength);

            if (received == SocketStatus::Error) {
                std::cerr << "The application will close." << std::endl;
                output.close();
                delete buff;
                return EXIT_FAILURE;
            } else if (received == SocketStatus::MoveFrame || received == SocketStatus::Normal) {
                int timesMoved = s.moveFrame();
                next = true;
                while (timesMoved > 0 && buff->nextAvailable(currentStart)) {
                    auto packet = buff->findPacket(currentStart);
                    if (processPacket(packet, currentStart, currentLength, output)) {
                        bytesLeft -= DATA_SIZE;
                        currentLength = bytesLeft >= DATA_SIZE ? DATA_SIZE : bytesLeft;
                        currentStart += currentLength;
                    }
                    timesMoved--;
                }
            } else if (received == SocketStatus::NothingReceived) {
                continue;
            }
        }
    }

    delete buff;
    output.close();

    return 0;
}

int processPacket(const packet &p, int currentStart, int currentLength, std::ofstream &file) {
    if (p.getStatus() > 0) {
        if (p.getStart() == currentStart && p.getLength() == currentLength) {
            writeToFile(p, file);
            return 1;
        }
    }

    return 0;
}

void writeToFile(const packet &p, std::ofstream &file) {
    if (file.is_open()) {
        file.write(p.getData().data(), p.getLength());
    } else {
        std::cout << "There was an error while writing to file. Please try again.\n";
        std::terminate();
    }
}

bool validateInput(int n, char **arguments) {
    if (n != 4) return false;

    serverPort = std::stoi(arguments[1]);
    outputFileName = arguments[2];
    fileLength = std::stoi(arguments[3]);

    if (fileLength < 0) return false;
    return !(serverPort < 0 || serverPort > 65535);
}