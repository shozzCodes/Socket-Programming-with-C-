# TCP Socket Programming — Full-Duplex Communication & Screenshot Transfer

A Windows-based **TCP socket programming project in C++** demonstrating client-server communication over a local network.

The project implements:

* TCP client-server communication
* Full-duplex messaging
* Multithreaded send/receive operations
* Communication between two physical laptops
* LAN communication over Wi-Fi or Ethernet
* Binary data transfer
* Screenshot capture and transfer
* Windows Winsock API
* A shared protocol definition between client and server

---

## 📌 Project Overview

This project demonstrates how two computers can communicate directly using **TCP sockets**.

One computer acts as the **server**, while another acts as the **client**.

```text
┌─────────────────────────┐
│       SERVER LAPTOP     │
│                         │
│  C++ TCP Server         │
│  Port: 5000             │
│                         │
│  192.168.x.x             │
└────────────┬────────────┘
             │
             │ TCP
             │
             ▼
┌─────────────────────────┐
│       CLIENT LAPTOP     │
│                         │
│  C++ TCP Client         │
│                         │
│  192.168.x.x             │
└─────────────────────────┘
```

The connection can operate over:

* Wi-Fi
* Ethernet through a router/switch
* Direct Ethernet connection between two laptops

The application supports **full-duplex communication**, meaning both computers can send and receive messages simultaneously.

The final stage also supports transferring a screenshot from the client to the server.

---

# 📂 Project Structure

The project is organized as follows:

```text
Socket Programming/
│
├── common/
│   └── protocol.h
│
├── server/
│   ├── server_stage6.cpp
│   └── server6.exe
│
├── client/
│   ├── client_stage6.cpp
│   └── client6.exe
│
└── README.md
```

### `common/`

Contains files shared by both client and server.

`protocol.h` defines the communication protocol used by both sides.

### `server/`

Contains the server implementation.

The server:

* Creates the listening socket
* Binds to the local network interfaces
* Listens for incoming TCP connections
* Accepts the client
* Sends and receives messages
* Handles screenshot-related communication
* Uses a receiver thread for full-duplex communication

### `client/`

Contains the client implementation.

The client:

* Creates a TCP socket
* Connects to the server
* Sends and receives messages
* Handles screenshot requests/data
* Uses a receiver thread for simultaneous communication

---

# 🛠️ Requirements

This project is designed for:

* Windows 10/11
* 64-bit x86 systems
* MinGW-w64
* GCC/G++ 14 or newer recommended
* MSYS2 UCRT64 environment

No external IDE is required.

You can compile the project entirely from the terminal.

---

# ⚙️ 1. Install the C++ Compiler From Scratch

The recommended compiler environment is:

**MSYS2 + MinGW-w64 UCRT64**

Official MSYS2 website:

https://www.msys2.org/

Download and install MSYS2 using the default installation location unless you have a reason to change it.

After installation, open:

```text
MSYS2 UCRT64
```

from the Windows Start Menu.

> Do not use the old MinGW.org GCC 6.x compiler.

---

# ⚙️ 2. Update MSYS2

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

# ⚙️ 3. Install GCC / G++

Install the MinGW-w64 UCRT64 compiler:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

Press:

```text
Y
```

when asked for confirmation.

Verify the installation:

```bash
g++ --version
```

A modern installation should show a MinGW-w64 UCRT64 compiler.

For example:

```text
g++.exe ... x86_64-ucrt-posix-seh
```

---

# ⚙️ 4. Verify the Compiler

Run:

```bash
where g++
```

Make sure Windows is using the newly installed MinGW-w64 compiler.

A modern MSYS2 installation should point to an MSYS2 UCRT64 `bin` directory.

You can also check:

```bash
g++ --version
```

The project requires a compiler capable of supporting:

* C++11+
* `std::thread`
* modern Winsock headers
* C++17 features used by the project

---

# 📥 5. Clone the Repository

Install Git if it is not already installed.

Official Git website:

https://git-scm.com/

Then clone the repository:

```bash
git clone <YOUR-GITHUB-REPOSITORY-URL>
```

Enter the project:

```bash
cd "Socket Programming"
```

The folder structure should remain unchanged.

---

# 📁 6. Verify the Folder Structure

Before compiling, make sure you have:

```text
Socket Programming/
│
├── common/
│   └── protocol.h
│
├── server/
│   └── server_stage6.cpp
│
└── client/
    └── client_stage6.cpp
```

This structure is important because the source files use a relative path such as:

```cpp
#include "../common/protocol.h"
```

The `../` means:

> Go one directory up, then enter the `common` directory.

---

# 🖥️ 7. Compile the Server

Open a terminal inside:

```text
Socket Programming/server
```

For example:

```powershell
cd "C:\Socket Programming\server"
```

Verify that the source file exists:

```powershell
dir
```

You should see:

```text
server_stage6.cpp
```

Compile:

```bash
g++ server_stage6.cpp -o server6.exe -lws2_32 -static -pthread
```

If compilation succeeds, no error message is normally displayed.

You should now have:

```text
server6.exe
```

---

# 💻 8. Compile the Client

Open another terminal inside:

```text
Socket Programming/client
```

For example:

```powershell
cd "C:\Socket Programming\client"
```

Compile:

```bash
g++ client_stage6.cpp -o client6.exe -lws2_32 -static -pthread
```

You should now have:

```text
client6.exe
```

---

# 🌐 9. Connect the Two Laptops

Both computers must be able to communicate over the same local network.

You can use:

### Wi-Fi

```text
Laptop A ───── Wi-Fi ───── Router
                           │
Laptop B ───── Wi-Fi ─────┘
```

### Ethernet + Router/Switch

```text
Laptop A ─── Ethernet ─── Switch/Router
                              │
Laptop B ─── Ethernet ────────┘
```

### Direct Ethernet

```text
Laptop A ───────── Ethernet ───────── Laptop B
```

For a direct connection, static IP addresses may need to be configured manually.

---

# 🔍 10. Find the Server IP Address

On the laptop running the server, open Command Prompt or PowerShell:

```cmd
ipconfig
```

Find the IPv4 address of the active network adapter.

Example:

```text
IPv4 Address : 192.168.0.111
```

The server's IP address will vary depending on the network.

**Do not assume the example IP is your actual IP.**

---

# 🔧 11. Configure the Client IP

Open:

```text
client/client_stage6.cpp
```

Find the server IP configuration.

For example:

```cpp
#define SERVER_IP "192.168.0.111"
```

Replace the address with the **current IPv4 address of the server laptop**.

Example:

```cpp
#define SERVER_IP "192.168.1.25"
```

The port must match the server:

```cpp
#define PORT 5000
```

After changing the client source code, recompile it:

```bash
g++ client_stage6.cpp -o client6.exe -lws2_32 -static -pthread
```

---

# 🔥 12. Configure Windows Firewall

The server needs to accept incoming TCP connections on port `5000`.

Windows Firewall should remain enabled.

Instead of disabling the entire firewall, create a specific inbound rule for TCP port 5000.

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

Verify the rule:

```powershell
Get-NetFirewallRule -DisplayName "Socket Server TCP 5000"
```

The rule should be enabled and allow inbound traffic.

> Only create firewall rules appropriate for a trusted network and remove the custom rule when it is no longer needed.

---

# 🧪 13. Test Network Connectivity

Before running the client, test basic network connectivity.

From the client laptop:

```cmd
ping SERVER_IP
```

For example:

```cmd
ping 192.168.0.111
```

Successful replies indicate that the laptops can communicate at the IP level.

---

# 🔌 14. Test TCP Port 5000

From the client laptop, run:

```powershell
Test-NetConnection SERVER_IP -Port 5000
```

Example:

```powershell
Test-NetConnection 192.168.0.111 -Port 5000
```

Look for:

```text
TcpTestSucceeded : True
```

If it says:

```text
TcpTestSucceeded : False
```

do not immediately change the C++ code.

Check:

1. The server is running.
2. The server is listening on port 5000.
3. The server IP is correct.
4. Both laptops are on the same network.
5. Windows Firewall allows TCP 5000.
6. The network is not isolating wireless clients.

---

# 🖥️ 15. Start the Server

On the server laptop:

```powershell
cd "C:\Socket Programming\server"
```

Run:

```powershell
.\server6.exe
```

The server should begin listening for a client.

Keep this terminal open.

---

# 💻 16. Start the Client

On the client laptop:

```powershell
cd "C:\Socket Programming\client"
```

Run:

```powershell
.\client6.exe
```

The client should connect to the server's IP address on port `5000`.

---

# 🔄 Full-Duplex Communication

The project uses a separate receiver thread so sending and receiving can happen concurrently.

Conceptually:

```text
                 TCP CONNECTION
              ┌──────────────────┐
              │                  │
              │                  │
        SEND ─┤                  ├─ RECEIVE
              │                  │
      RECEIVE ├                  ├─ SEND
              │                  │
              └──────────────────┘
```

The main thread handles sending while a separate thread waits for incoming data.

This allows both computers to communicate without requiring a strict:

```text
send → receive → send → receive
```

sequence.

Instead, both sides can communicate independently.

---

# 📸 Screenshot Transfer

The final stage also demonstrates transferring screenshot data between the client and server.

The general process is:

```text
Client
   │
   │ Capture screen
   ▼
Screenshot data
   │
   │ Convert to bytes
   ▼
TCP socket
   │
   │ Transfer
   ▼
Server
   │
   │ Receive data
   ▼
Reconstruct screenshot
   │
   ▼
Save/display screenshot
```

Because screenshots are binary data, the implementation must correctly handle data larger than a single TCP `send()` or `recv()` call.

TCP provides a byte stream rather than individual message boundaries, so the application protocol is responsible for determining how much data belongs to a screenshot.

---

# 🧠 Important TCP Concept

A common beginner mistake is assuming:

```cpp
recv(socket, buffer, 4096, 0);
```

means:

> "Receive the entire message."

It does not.

`recv()` returns the number of bytes currently received, which may be less than the total amount of data being transferred.

Therefore, large data such as screenshots must be transferred using an appropriate protocol and repeated send/receive operations.

This project demonstrates that concept through the screenshot-transfer stage.

---

# 🛑 Stopping the Program

Normally, use the application's built-in shutdown/quit mechanism.

If the program becomes stuck during development, you can terminate it with:

```text
Ctrl + C
```

---

# 🧹 Remove the Firewall Rule

After the project/demo is finished, the custom firewall rule can be removed.

Open PowerShell as Administrator:

```powershell
Remove-NetFirewallRule -DisplayName "Socket Server TCP 5000"
```

Windows Firewall itself should remain enabled.

---

# 🐛 Troubleshooting

## `g++ is not recognized`

Check:

```bash
g++ --version
```

If it isn't found, make sure you are using the **MSYS2 UCRT64** terminal and that MinGW-w64 GCC is installed.

Install it with:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
```

---

## `protocol.h: No such file or directory`

Check the project structure:

```text
Socket Programming/
├── common/
│   └── protocol.h
├── server/
│   └── server_stage6.cpp
└── client/
    └── client_stage6.cpp
```

Compile from inside the appropriate directory.

For example:

```text
Socket Programming/server
```

and not from an unrelated directory.

---

## `undefined reference` / Winsock linker errors

Make sure the Winsock library is linked:

```bash
-lws2_32
```

Example:

```bash
g++ server_stage6.cpp -o server6.exe -lws2_32 -static -pthread
```

---

## `10060` connection timeout

Error `10060` generally means the client could not establish the TCP connection within the timeout period.

Check:

```text
1. Is the server running?
2. Is the server IP correct?
3. Is the server listening on port 5000?
4. Are both laptops on the same network?
5. Is Windows Firewall allowing TCP 5000?
6. Does Test-NetConnection report True?
```

On the server:

```cmd
netstat -ano | findstr :5000
```

You should see something similar to:

```text
TCP    0.0.0.0:5000    0.0.0.0:0    LISTENING
```

If you see:

```text
127.0.0.1:5000
```

the server is listening only on localhost and other computers cannot connect.

The server should bind using:

```cpp
serverAddr.sin_addr.s_addr = INADDR_ANY;
```

---

## `Test-NetConnection` returns `False`

Run on the server:

```cmd
netstat -ano | findstr :5000
```

Then verify the firewall rule.

Also check that the client is connecting to the correct server IP:

```cmd
ipconfig
```

on the server.

---

# 🔐 Security Notes

This project is intended for **controlled educational/lab environments**.

The TCP connection itself is not automatically encrypted simply because it uses TCP.

Therefore:

* Use the project on networks you trust.
* Do not expose the socket port to the public Internet.
* Do not configure router port forwarding for this educational project.
* Keep Windows Firewall enabled.
* Use explicit consent when demonstrating screenshot capture.
* Avoid using the application to capture private information without permission.
* Remove unnecessary firewall rules after testing.

---

# 📚 Concepts Demonstrated

This project covers several important Computer Networks concepts:

### Network Layer

* IPv4 addressing
* Subnetting
* Local Area Networks
* IP connectivity

### Transport Layer

* TCP
* Ports
* Connection-oriented communication
* Reliable byte-stream transmission

### Socket Programming

* `socket()`
* `bind()`
* `listen()`
* `accept()`
* `connect()`
* `send()`
* `recv()`
* `shutdown()`
* `closesocket()`

### Windows Networking

* Winsock 2
* `WSAStartup()`
* `WSACleanup()`
* Windows Firewall

### Concurrency

* C++ `std::thread`
* Concurrent send/receive operations
* `std::atomic`

### Data Transfer

* Text messages
* Binary data
* TCP stream handling
* Screenshot transfer
* Application-level protocol design

---

# 🚀 Quick Demo

Once everything has been installed:

### Server laptop

```powershell
cd server
.\server6.exe
```

### Client laptop

Make sure `SERVER_IP` contains the server's current IP, then:

```powershell
cd client
.\client6.exe
```

Test the TCP connection if needed:

```powershell
Test-NetConnection SERVER_IP -Port 5000
```

Expected:

```text
TcpTestSucceeded : True
```

Then demonstrate:

1. Client → Server messaging
2. Server → Client messaging
3. Simultaneous/full-duplex communication
4. Screenshot transfer

---

# 📝 Notes for Demonstration on Another Laptop

The server's IP address is **not permanent**.

For every new network:

```cmd
ipconfig
```

on the server laptop and update the client configuration accordingly.

Example:

```text
Network 1:
Server = 192.168.0.111

Network 2:
Server = 192.168.1.25
```

The client must connect to the server's current IP.

The port remains:

```text
5000
```

unless changed in the source code.

---

# 👨‍💻 Author

**Mohammad Shozab Ali**

BS Computer Science

Computer Networks — Socket Programming Project

---

# 📄 License

This project was created for educational and academic purposes.

You are free to study and modify the code for learning purposes.
