# TCP Socket Programming — Full-Duplex Communication & Screenshot Transfer

A Windows-based **TCP socket programming project in C++** demonstrating real-time client-server communication between two physical computers over a Local Area Network (LAN).

The final implementation supports:

* TCP client-server communication
* Full-duplex messaging
* Multithreaded communication
* Communication between two physical laptops
* Wi-Fi LAN communication
* Ethernet LAN communication
* Binary data transfer
* Screenshot capture and transfer
* Custom application-level communication protocol
* Shared protocol definitions
* Windows Winsock API
* Windows Firewall configuration
* Graceful connection shutdown

---

# 📌 Project Overview

This project demonstrates how two computers can communicate using **TCP sockets**.

One computer runs the **server**, while another runs the **client**.

```text
                    LOCAL AREA NETWORK
                         TCP : 5000

        ┌──────────────────────────────┐
        │         SERVER LAPTOP        │
        │                              │
        │  server.cpp                  │
        │  server7.exe                 │
        │  Port: 5000                  │
        │                              │
        │  • Send messages             │
        │  • Receive messages           │
        │  • Receive screenshots       │
        └──────────────┬───────────────┘
                       │
                       │ TCP
                       │
        ┌──────────────▼───────────────┐
        │         CLIENT LAPTOP        │
        │                              │
        │  client.cpp                  │
        │  client7.exe                 │
        │                              │
        │  • Send messages             │
        │  • Receive messages           │
        │  • Capture screenshot        │
        │  • Transfer screenshot       │
        └──────────────────────────────┘
```

The two computers can communicate through:

* Wi-Fi
* Ethernet through a router
* Ethernet through a network switch
* Direct Ethernet connection between two laptops

The final project supports **full-duplex communication**, meaning both sides can send and receive data independently at the same time.

---

# ✨ Final Features

## 1. TCP Client-Server Communication

The project uses TCP sockets through the Windows Winsock API.

The server follows the standard TCP server sequence:

```text
WSAStartup()
     ↓
socket()
     ↓
bind()
     ↓
listen()
     ↓
accept()
     ↓
send()/recv()
```

The client follows:

```text
WSAStartup()
     ↓
socket()
     ↓
connect()
     ↓
send()/recv()
```

Once the connection is established, both sides communicate through the same TCP connection.

---

# 2. Full-Duplex Messaging

Both computers can send and receive messages independently.

```text
       SERVER                         CLIENT
          │                              │
          │────── "Hello" ──────────────►│
          │                              │
          │◄────── "Hi!" ────────────────│
          │                              │
          │────── "How are you?" ───────►│
          │◄────── "Good!" ──────────────│
          │                              │
```

The communication is not restricted to:

```text
send → receive → send → receive
```

Both sides can communicate concurrently.

---

# 3. Multithreaded Communication

The project uses C++:

```cpp
std::thread
```

to separate receiving from the main sending loop.

Conceptually:

```text
                  TCP SOCKET
                      │
            ┌─────────┴─────────┐
            │                   │
            ▼                   ▼
      Main Thread         Receiver Thread
            │                   │
         send()              recv()
            │                   │
            ▼                   ▼
      Outgoing data        Incoming data
```

This allows the application to remain responsive while waiting for incoming data.

An `std::atomic<bool>` variable is used to coordinate connection state between threads.

---

# 4. Screenshot Capture & Transfer

The final stage adds screenshot functionality.

The client can capture its screen and transfer the screenshot through the TCP connection.

The general process is:

```text
CLIENT
  │
  │ Capture screen
  ▼
Screenshot
  │
  │ Convert to transferable data
  ▼
Binary data
  │
  │ TCP
  ▼
SERVER
  │
  │ Receive binary data
  ▼
Reconstruct screenshot
  │
  ▼
Save/display screenshot
```

The screenshot functionality demonstrates that a TCP socket can transfer **binary data**, not just text messages.

The screenshot implementation is contained in:

```text
common/screenshot.h
```

---

# 5. Binary Data Transfer

Text messages are not the only type of information transferred by the project.

The screenshot feature requires binary data transmission.

A screenshot may contain a large amount of data, so the application handles the transfer as a sequence of bytes rather than assuming that the entire image will arrive in one `recv()` call.

This demonstrates an important TCP concept:

> TCP is a byte stream, not a message-based protocol.

For example:

```cpp
recv(socket, buffer, 4096, 0);
```

does **not** guarantee that the entire application-level message has been received.

The application protocol therefore determines how much data belongs to a particular transfer and when the complete screenshot has been received.

---

# 6. Custom Communication Protocol

The project contains:

```text
common/protocol.h
```

This file contains definitions shared by the client and server.

Both applications use the same protocol definitions so that they agree on how different types of data are exchanged.

The protocol allows the applications to distinguish between different types of operations and data.

---

# 7. Screenshot Utility

The project also contains:

```text
common/screenshot.h
```

This header provides the screenshot-related functionality used by the application.

Keeping screenshot functionality separate from the main client/server source code makes the project easier to organize and maintain.

---

# 📂 Final Project Structure

The final project uses the following structure:

```text
Socket Programming/
│
├── common/
│   ├── protocol.h
│   └── screenshot.h
│
├── server/
│   ├── server.cpp
│   └── server7.exe
│
├── client/
│   ├── client.cpp
│   └── client7.exe
│
└── README.md
```

### `common/protocol.h`

Contains communication protocol definitions shared by the client and server.

### `common/screenshot.h`

Contains screenshot capture functionality used by the project.

### `server/server.cpp`

Contains the TCP server implementation.

### `server/server7.exe`

Compiled server executable.

### `client/client.cpp`

Contains the TCP client implementation.

### `client/client7.exe`

Compiled client executable.

---

# 🛠️ System Requirements

## Operating System

* Windows 10
* Windows 11

## Architecture

* 64-bit x86 Windows systems

## Compiler

Recommended:

* MinGW-w64
* UCRT64
* GCC/G++ 14 or newer

## Libraries / APIs

The project uses:

* Windows Winsock 2
* Windows screenshot APIs
* C++ Standard Library
* C++ threading support

Winsock is linked using:

```text
-lws2_32
```

No external networking library is required.

---

# ⚙️ Installing the Development Environment From Scratch

## Step 1 — Install MSYS2

Download MSYS2 from the official website:

https://www.msys2.org/

Install MSYS2 on the Windows computer.

After installation, open:

```text
MSYS2 UCRT64
```

from the Windows Start Menu.

> Use the **UCRT64** environment for this project.

---

# Step 2 — Update MSYS2

Inside the **MSYS2 UCRT64** terminal, run:

```bash
pacman -Syu
```

If MSYS2 asks you to close the terminal, close it.

Open **MSYS2 UCRT64** again and run:

```bash
pacman -Syu
```

---

# Step 3 — Install MinGW-w64 GCC

Install the compiler:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Confirm the installation when prompted.

---

# Step 4 — Verify GCC

Run:

```bash
g++ --version
```

A correct installation should identify itself as a MinGW-w64 UCRT64 compiler.

For example:

```text
x86_64-ucrt-posix-seh
```

The exact GCC version may be newer than the version used during development.

---

# Step 5 — Install Git

Git is recommended for cloning the repository.

Download Git from:

https://git-scm.com/

Verify the installation:

```bash
git --version
```

---

# 📥 Getting the Project

Clone the repository:

```bash
git clone <YOUR-GITHUB-REPOSITORY-URL>
```

Enter the project directory:

```bash
cd "Socket Programming"
```

---

# 📁 Verify the Project Structure

Before compiling, make sure the project contains:

```text
Socket Programming/
│
├── common/
│   ├── protocol.h
│   └── screenshot.h
│
├── server/
│   └── server.cpp
│
└── client/
    └── client.cpp
```

This structure is important because the source files use relative paths such as:

```cpp
#include "../common/protocol.h"
#include "../common/screenshot.h"
```

The `../` means:

> Go one directory up, then enter the `common` directory.

---

# 🖥️ Compiling the Server

Open a terminal inside:

```text
Socket Programming/server
```

For example:

```powershell
cd "C:\Socket Programming\server"
```

Compile:

```bash
g++ server.cpp -o server7.exe -lws2_32 -static -pthread
```

If compilation succeeds, the executable will be created:

```text
server7.exe
```

---

# 💻 Compiling the Client

Open a terminal inside:

```text
Socket Programming/client
```

For example:

```powershell
cd "C:\Socket Programming\client"
```

Compile:

```bash
g++ client.cpp -o client7.exe -lws2_32 -static -pthread
```

The executable will be:

```text
client7.exe
```

---

# 🌐 Network Setup

Both laptops must be able to communicate over a local network.

## Option 1 — Wi-Fi

```text
Laptop A
   │
   │ Wi-Fi
   ▼
Router
   ▲
   │ Wi-Fi
   │
Laptop B
```

Both laptops should be connected to the same LAN.

---

## Option 2 — Ethernet + Router/Switch

```text
Laptop A
   │
   │ Ethernet
   ▼
Router / Switch
   ▲
   │ Ethernet
   │
Laptop B
```

---

## Option 3 — Direct Ethernet

```text
Laptop A
   │
   │ Ethernet cable
   │
Laptop B
```

For a direct Ethernet connection, static IPv4 addresses may be required because there may be no DHCP server.

Example:

### Server

```text
IP Address:   192.168.0.111
Subnet Mask:  255.255.255.0
Gateway:      leave blank
```

### Client

```text
IP Address:   192.168.0.110
Subnet Mask:  255.255.255.0
Gateway:      leave blank
```

---

# 🔍 Finding the Server IP

On the server laptop:

```cmd
ipconfig
```

Find the IPv4 address of the active network adapter.

Example:

```text
IPv4 Address : 192.168.0.111
```

The actual address will depend on the network.

---

# 🔧 Configure the Client

Open:

```text
client/client.cpp
```

Find the server IP configuration.

For example:

```cpp
#define SERVER_IP "192.168.0.111"
```

Replace it with the **current IPv4 address of the server laptop**.

Example:

```cpp
#define SERVER_IP "192.168.1.25"
```

The port must match the server:

```cpp
#define PORT 5000
```

After changing the IP, recompile the client:

```bash
g++ client.cpp -o client7.exe -lws2_32 -static -pthread
```

---

# 🔥 Windows Firewall Configuration

The server must accept incoming TCP connections on port `5000`.

**Do not disable the entire Windows Firewall.**

Instead, create a specific inbound firewall rule.

Open **PowerShell as Administrator** on the server laptop.

Run:

```powershell
New-NetFirewallRule `
    -DisplayName "Socket Server TCP 5000" `
    -Direction Inbound `
    -Protocol TCP `
    -LocalPort 5000 `
    -Action Allow `
    -Profile Private
```

Verify:

```powershell
Get-NetFirewallRule -DisplayName "Socket Server TCP 5000"
```

### File and Printer Sharing

**File and Printer Sharing is NOT required.**

The project communicates directly using Winsock/TCP and does not depend on Windows SMB/file-sharing functionality.

---

# 🧪 Testing the Network

Before starting the client, test basic connectivity.

From the client laptop:

```cmd
ping SERVER_IP
```

Example:

```cmd
ping 192.168.0.111
```

Then test TCP port `5000`:

```powershell
Test-NetConnection SERVER_IP -Port 5000
```

Example:

```powershell
Test-NetConnection 192.168.0.111 -Port 5000
```

Expected:

```text
TcpTestSucceeded : True
```

---

# 🖥️ Starting the Server

On the server laptop:

```powershell
cd "C:\Socket Programming\server"
```

Run:

```powershell
.\server7.exe
```

The server will begin listening for incoming connections.

Keep the terminal open.

---

# 💻 Starting the Client

On the client laptop:

```powershell
cd "C:\Socket Programming\client"
```

Run:

```powershell
.\client7.exe
```

The client should connect to the configured server IP on port `5000`.

---

# 🔄 Demonstrating Full-Duplex Communication

Once connected:

### Client → Server

Type a message on the client.

The server should receive it.

### Server → Client

Type a message on the server.

The client should receive it.

### Simultaneous Communication

Both sides can send messages without waiting for the other side to finish receiving.

This demonstrates **full-duplex TCP communication**.

---

# 📸 Demonstrating Screenshot Transfer

The final implementation supports screenshot transfer from the client.

The demonstration flow is:

```text
1. Start server
        ↓
2. Start client
        ↓
3. Establish TCP connection
        ↓
4. Perform full-duplex messaging
        ↓
5. Trigger screenshot functionality
        ↓
6. Client captures screenshot
        ↓
7. Screenshot is converted into transferable data
        ↓
8. Screenshot data is transferred over TCP
        ↓
9. Server receives the binary data
        ↓
10. Screenshot is reconstructed/saved
```

The screenshot feature demonstrates the transfer of binary information through a TCP connection.

---

# 🧠 Important TCP Concept — Byte Streams

TCP is a **byte-stream protocol**.

For example:

```cpp
send(socket, data, size, 0);
```

does not guarantee that the receiver will obtain all `size` bytes in a single:

```cpp
recv()
```

call.

For large transfers such as screenshots, the application must correctly handle multiple send/receive operations and determine when the complete application-level data has been received.

This is one of the important networking concepts demonstrated by the project.

---

# 🧵 Threading Architecture

The client and server use separate receiving threads.

Conceptually:

```text
                     TCP SOCKET
                         │
               ┌─────────┴─────────┐
               │                   │
               ▼                   ▼
         Main Thread         Receiver Thread
               │                   │
             send()              recv()
               │                   │
               ▼                   ▼
         Outgoing data        Incoming data
```

This allows sending and receiving to happen concurrently.

An atomic connection-state variable is used to safely coordinate shutdown between threads.

---

# 🔌 Important Socket Functions

## Server

```text
WSAStartup()
     ↓
socket()
     ↓
bind()
     ↓
listen()
     ↓
accept()
     ↓
send()/recv()
     ↓
shutdown()
     ↓
closesocket()
     ↓
WSACleanup()
```

## Client

```text
WSAStartup()
     ↓
socket()
     ↓
connect()
     ↓
send()/recv()
     ↓
shutdown()
     ↓
closesocket()
     ↓
WSACleanup()
```

---

# 🐛 Troubleshooting

## Error `10060`

If the client reports:

```text
connect() failed: 10060
```

check:

1. Is `server7.exe` running?
2. Is the server IP correct?
3. Are both laptops on the same LAN?
4. Is TCP port `5000` allowed through Windows Firewall?
5. Does `Test-NetConnection` return `True`?
6. Is the server actually listening on port `5000`?

On the server:

```cmd
netstat -ano | findstr :5000
```

A working server should show something similar to:

```text
TCP    0.0.0.0:5000    0.0.0.0:0    LISTENING
```

### Important

If the server shows:

```text
127.0.0.1:5000
```

then it is listening only on the local computer.

For LAN communication, the server should bind using:

```cpp
serverAddr.sin_addr.s_addr = INADDR_ANY;
```

---

# `TcpTestSucceeded : False`

If:

```text
TcpTestSucceeded : False
```

check:

```text
Server running?
      ↓
Correct server IP?
      ↓
Same LAN?
      ↓
Server listening on port 5000?
      ↓
Firewall rule configured?
      ↓
Network isolation?
```

Do not immediately modify the C++ networking code.

First establish that the TCP connection itself works.

---

# `g++ is not recognized`

Verify:

```bash
g++ --version
```

If GCC is unavailable, install:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Also make sure the **MSYS2 UCRT64** terminal is being used.

---

# `protocol.h` or `screenshot.h` Not Found

Verify:

```text
Socket Programming/
│
├── common/
│   ├── protocol.h
│   └── screenshot.h
│
├── server/
│   └── server.cpp
│
└── client/
    └── client.cpp
```

Compile from inside the appropriate `server` or `client` directory.

---

# Winsock Linker Errors

Make sure the compile command includes:

```text
-lws2_32
```

Server:

```bash
g++ server.cpp -o server7.exe -lws2_32 -static -pthread
```

Client:

```bash
g++ client.cpp -o client7.exe -lws2_32 -static -pthread
```

---

# 🛑 Stopping the Application

Use the application's normal quit/shutdown mechanism.

If necessary, press:

```text
Ctrl + C
```

in the terminal running the application.

---

# 🧹 Removing the Firewall Rule

After completing the demonstration, the custom firewall rule can be removed.

Open PowerShell as Administrator:

```powershell
Remove-NetFirewallRule -DisplayName "Socket Server TCP 5000"
```

Windows Firewall itself should remain enabled.

---

# 🔐 Security & Privacy

This project is intended for **educational and controlled LAN demonstrations**.

The application should only be used with the knowledge and authorization of the people involved.

Important considerations:

* TCP does not automatically encrypt application data.
* Do not expose port `5000` directly to the public Internet.
* Do not configure router port forwarding for this project.
* Keep Windows Firewall enabled.
* Use a trusted LAN for demonstrations.
* Screenshot functionality should only be used with appropriate consent.
* Screenshots may contain passwords, private messages, documents, or other sensitive information.
* Remove temporary firewall rules after testing.

---

# 📚 Concepts Demonstrated

This project combines several Computer Networks concepts.

## Networking

* IPv4 addressing
* LAN communication
* Subnet masks
* Ports
* TCP/IP
* Client-server architecture

## Transport Layer

* TCP
* Connection-oriented communication
* Reliable byte-stream transmission
* TCP ports

## Socket Programming

* `socket()`
* `bind()`
* `listen()`
* `accept()`
* `connect()`
* `send()`
* `recv()`
* `shutdown()`
* `closesocket()`

## Windows Networking

* Winsock 2
* `WSAStartup()`
* `WSACleanup()`
* Windows Firewall

## C++

* `std::thread`
* `std::atomic`
* Standard library
* Binary data handling
* File operations

## Application Protocol

* Message types
* Data framing
* Binary transfer
* Screenshot transfer
* Client-server protocol design

---

# 👨‍💻 Author

**Mohammad Shozab Ali**

BS Computer Science

**Computer Networks — TCP Socket Programming Project**

---

# 📄 License

This project was created for educational and academic purposes.

You are free to study and modify the code for learning purposes.
