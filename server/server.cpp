#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <shellapi.h>
#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <atomic>
#include <ctime>
#include "../common/protocol.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "shell32.lib")

#define PORT 5000

std::atomic<bool> connectionActive(true);


void saveAndDisplayScreenshot(const std::vector<char>& bmpBytes) {
    if (bmpBytes.empty()) {
        std::cout << "[SERVER] Client reported screenshot capture FAILED (empty data)." << std::endl;
        return;
    }

    std::time_t now = std::time(nullptr);
    std::string filename = "screenshot_" + std::to_string(now) + ".bmp";

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cout << "[SERVER] Failed to open file for writing: " << filename << std::endl;
        return;
    }
    outFile.write(bmpBytes.data(), bmpBytes.size());
    outFile.close();

    std::cout << "[SERVER] Screenshot saved: " << filename
              << " (" << bmpBytes.size() << " bytes). Opening..." << std::endl;

    // Opens with the default image viewer. Non-fatal if this fails - the
    // file is already safely saved either way.
    ShellExecuteA(NULL, "open", filename.c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void receiverThreadFunc(SOCKET clientSocket) {
    while (connectionActive) {
        MsgType type;
        std::vector<char> data;

        if (!recvMessage(clientSocket, type, data)) {
            std::cout << "\n[SERVER] Connection closed by client (or error)." << std::endl;
            connectionActive = false;
            break;
        }

        switch (type) {
            case MsgType::TEXT: {
                std::string text(data.begin(), data.end());
                std::cout << "\n[CLIENT SAYS] " << text << "\n[SERVER] > " << std::flush;
                break;
            }
            case MsgType::QUIT:
                std::cout << "\n[SERVER] Client sent QUIT." << std::endl;
                connectionActive = false;
                break;
            case MsgType::SCREENSHOT_DATA:
                std::cout << std::endl;
                saveAndDisplayScreenshot(data);
                std::cout << "[SERVER] > " << std::flush;
                break;
            default:
                std::cout << "\n[SERVER] Received unknown message type: "
                          << static_cast<uint32_t>(type) << std::endl;
                break;
        }
    }
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed: " << WSAGetLastError() << std::endl;
        return 1;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "bind() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    if (listen(listenSocket, 5) == SOCKET_ERROR) {
        std::cerr << "listen() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "[SERVER] Listening on 0.0.0.0:" << PORT << " ..." << std::endl;

    sockaddr_in clientAddr{};
    int clientAddrLen = sizeof(clientAddr);
    SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "accept() failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
    std::cout << "[SERVER] Client connected from " << clientIP
              << ":" << ntohs(clientAddr.sin_port) << std::endl;
    std::cout << "Type messages and press Enter to send.\n"
              << "Type 'screenshot' to request the client's desktop.\n"
              << "Type 'quit' to exit.\n" << std::endl;

    std::thread receiver(receiverThreadFunc, clientSocket);

    std::string line;
    while (connectionActive) {
        std::cout << "[SERVER] > " << std::flush;
        if (!std::getline(std::cin, line)) break;

        if (line == "quit") {
            sendMessage(clientSocket, MsgType::QUIT, nullptr, 0);
            connectionActive = false;
            break;
        } else if (line == "screenshot") {
            std::cout << "[SERVER] Requesting screenshot from client..." << std::endl;
            if (!sendMessage(clientSocket, MsgType::SCREENSHOT_REQUEST, nullptr, 0)) {
                connectionActive = false;
                break;
            }
        } else if (!line.empty()) {
            if (!sendMessage(clientSocket, MsgType::TEXT, line.c_str(),
                              static_cast<uint32_t>(line.size()))) {
                connectionActive = false;
                break;
            }
        }
    }

    shutdown(clientSocket, SD_BOTH);
    closesocket(clientSocket);

    if (receiver.joinable()) receiver.join();

    closesocket(listenSocket);
    WSACleanup();

    std::cout << "[SERVER] Shutdown complete." << std::endl;
    return 0;
}