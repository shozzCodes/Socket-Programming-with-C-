
#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <cstdint>
#include <vector>
#include <iostream>


enum class MsgType : uint32_t {
    TEXT               = 1,  // plain chat message
    SCREENSHOT_REQUEST = 2,  // server to client: "capture and send your desktop"
    SCREENSHOT_DATA    = 3,  // client to server: image bytes follow
    QUIT               = 4   // either side: "I'm closing the connection"
};

inline bool recvAll(SOCKET sock, char* buffer, int length) {
    int totalReceived = 0;
    while (totalReceived < length) {
        int bytesReceived = recv(sock, buffer + totalReceived, length - totalReceived, 0);
        if (bytesReceived == 0) {
            // Peer closed the connection cleanly (orderly shutdown).
            return false;
        }
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "recvAll(): recv() failed: " << WSAGetLastError() << std::endl;
            return false;
        }
        totalReceived += bytesReceived;
    }
    return true;
}


inline bool sendAll(SOCKET sock, const char* buffer, int length) {
    int totalSent = 0;
    while (totalSent < length) {
        int bytesSent = send(sock, buffer + totalSent, length - totalSent, 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "sendAll(): send() failed: " << WSAGetLastError() << std::endl;
            return false;
        }
        totalSent += bytesSent;
    }
    return true;
}


inline bool sendMessage(SOCKET sock, MsgType type, const char* data, uint32_t length) {
    uint32_t header[2];
    header[0] = htonl(static_cast<uint32_t>(type));
    header[1] = htonl(length);

    if (!sendAll(sock, reinterpret_cast<const char*>(header), sizeof(header))) {
        return false;
    }
    if (length > 0) {
        return sendAll(sock, data, static_cast<int>(length));
    }
    return true;
}


// recvMessage(): reads the 8-byte header first, THEN reads exactly
inline bool recvMessage(SOCKET sock, MsgType& outType, std::vector<char>& outData) {
    uint32_t header[2];
    if (!recvAll(sock, reinterpret_cast<char*>(header), sizeof(header))) {
        return false; // connection closed or error while waiting for header
    }

    outType = static_cast<MsgType>(ntohl(header[0]));
    uint32_t length = ntohl(header[1]);

    outData.resize(length);
    if (length > 0) {
        if (!recvAll(sock, outData.data(), static_cast<int>(length))) {
            return false; // connection dropped mid-payload
        }
    }
    return true;
}

#endif
