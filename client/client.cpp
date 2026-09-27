#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include "../common/protocol.h"
#include "../common/screenshot.h"

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "192.168.0.111"  // replace with server laptop's IP
#define PORT 5000

std::atomic<bool> connectionActive(true);

void handleScreenshotRequest(SOCKET sock) {
 
    std::cout << "\n[CLIENT] *** Server requested a screenshot - capturing desktop now ***"
              << std::endl;

    std::vector<char> bmpBytes;
    if (!captureDesktopAsBMP(bmpBytes)) {
        std::cout << "[CLIENT] Screenshot capture FAILED." << std::endl;
        // Tell the server it failed rather than silently doing nothing -
        sendMessage(sock, MsgType::SCREENSHOT_DATA, nullptr, 0);
        return;
    }

    std::cout << "[CLIENT] Captured " << bmpBytes.size() << " bytes. Sending to server..."
              << std::endl;

    if (!sendMessage(sock, MsgType::SCREENSHOT_DATA, bmpBytes.data(),
                      static_cast<uint32_t>(bmpBytes.size()))) {
        std::cout << "[CLIENT] Failed to send screenshot data." << std::endl;
    } else {
        std::cout << "[CLIENT] Screenshot sent successfully.\n[CLIENT] > " << std::flush;
    }
}

void receiverThreadFunc(SOCKET sock) {
    while (connectionActive) {
        MsgType type;
        std::vector<char> data;

        if (!recvMessage(sock, type, data)) {
            std::cout << "\n[CLIENT] Connection closed by server (or error)." << std::endl;
            connectionActive = false;
            break;
        }

        switch (type) {
            case MsgType::TEXT: {
                std::string text(data.begin(), data.end());
                std::cout << "\n[SERVER SAYS] " << text << "\n[CLIENT] > " << std::flush;
                break;
            }
            case MsgType::QUIT:
                std::cout << "\n[CLIENT] Server sent QUIT." << std::endl;
                connectionActive = false;
                break;
            case MsgType::SCREENSHOT_REQUEST:
                handleScreenshotRequest(sock);
                break;
            default:
                std::cout << "\n[CLIENT] Received unknown message type: "
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

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket() failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr) != 1) {
        std::cerr << "Invalid server IP address." << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "connect() failed: " << WSAGetLastError()
                  << " (is the server running and reachable?)" << std::endl;
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[CLIENT] Connected to server at " << SERVER_IP << ":" << PORT << std::endl;
    std::cout << "Type messages and press Enter to send. Type 'quit' to exit.\n" << std::endl;

    std::thread receiver(receiverThreadFunc, sock);

    std::string line;
    while (connectionActive) {
        std::cout << "[CLIENT] > " << std::flush;
        if (!std::getline(std::cin, line)) break;

        if (line == "quit") {
            sendMessage(sock, MsgType::QUIT, nullptr, 0);
            connectionActive = false;
            break;
        }
        if (!line.empty()) {
            if (!sendMessage(sock, MsgType::TEXT, line.c_str(),
                              static_cast<uint32_t>(line.size()))) {
                connectionActive = false;
                break;
            }
        }
    }

    shutdown(sock, SD_BOTH);
    closesocket(sock);

    if (receiver.joinable()) receiver.join();

    WSACleanup();

    std::cout << "[CLIENT] Shutdown complete." << std::endl;
    return 0;
}