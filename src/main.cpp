//Sebastian Paulus 266446
#include "utils.h"
#include "udpsocket.h"
#include "packetbuffer.h"

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
    bool nextFrame = true;
    bool next;

    std::ofstream output;
//    output.open(outputFileName, std::ios::binary | std::ios::out | std::ios::ate | std::ios::app);
    output.open(outputFileName);
    packetbuffer *buff = new packetbuffer();
    udpsocket s(buff, serverAddr, serverPort);

    if (s.run() < 0) {
        std::cerr << "The application will close." << std::endl;
        output.close();
        delete buff;
        return EXIT_FAILURE;
    }

    std::cout << "Receiving data.\n";

    std::chrono::steady_clock::time_point start, end;

    while (bytesLeft > 0) {
        int currentLength = bytesLeft >= FRAME_SIZE ? FRAME_SIZE : bytesLeft;
        next = false;

        if (nextFrame) {
            double progress = (fileLength - bytesLeft) / (fileLength * 1.0) * 100.0;
            std::cout << progress << "%\n";
            nextFrame = false;
        }
        int sent = (int) s.sendPacket(currentStart, currentLength);
        if (sent < 0) {
            std::cerr << "The application will close." << std::endl;
            output.close();
            delete buff;
            return EXIT_FAILURE;
        }
        start = std::chrono::steady_clock::now();
        end = std::chrono::steady_clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        while (!next && (elapsed.count() < TIMEOUT)) {
            end = std::chrono::steady_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

            auto received = s.getPacket(currentStart, currentLength);

            if (received == SocketStatus::Error) {
                std::cerr << "The application will close." << std::endl;
                output.close();
                delete buff;
                return EXIT_FAILURE;
            } else if (received == SocketStatus::MoveFrame || received == SocketStatus::Normal) {
                next = true;
                if (received == SocketStatus::MoveFrame) {
                    nextFrame = true;
                }
                int timesMoved = s.moveFrame();
                for (int i = 0; i < timesMoved; i++) {
                    auto packet = buff->findPacket(currentStart);
                    if (processPacket(packet, currentStart, currentLength, output)) {
                        bytesLeft -= FRAME_SIZE;
                        currentStart += FRAME_SIZE;
                        currentLength = bytesLeft >= FRAME_SIZE ? FRAME_SIZE : bytesLeft;
                    }
                }
            } else if (received == SocketStatus::NothingReceived) {
                continue;
            }
        }
    }
    std::cout << "Done.\n";

    delete buff;
    output.close();

    return 0;
}

int processPacket(const packet &p, int currentStart, int currentLength, std::ofstream &file) {
    if (p.getStatus() > 0) {
        std::string packetData = p.getData();
        if (p.getStart() == currentStart && p.getLength() == currentLength) {
            writeToFile(p, file);
            return 1;
        }
    }

    return 0;
}

void writeToFile(const packet &p, std::ofstream &file) {
    if (file.is_open()) {
        file.write(p.getData().c_str(), p.getLength());
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