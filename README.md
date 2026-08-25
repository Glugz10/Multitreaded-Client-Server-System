# Multithreaded TCP Chat Server

A C++17 client-server chat application built on Linux using POSIX TCP sockets and multithreading.

The project is designed to practice systems programming concepts including socket communication, concurrency, synchronization, shared resources, and client-server architecture.

## Features

- TCP client-server communication
- Multiple concurrent client connections
- Multithreaded server using `std::thread`
- Thread-safe shared client management using `std::mutex`
- Usernames for connected clients
- Message broadcasting
- `/users` command to view connected users
- `/quit` command for client disconnection
- Graceful handling of disconnected clients

## Technologies

- C++17
- Linux
- POSIX Sockets
- TCP/IP
- `std::thread`
- `std::mutex`
- `std::lock_guard`

## Project Structure

```text
multithreaded-chat-server/
├── server.cpp
├── client.cpp
├── README.md
├── Makefile
└── .gitignore
```

## How It Works

The server creates a TCP socket and listens for incoming client connections.

For each connected client, the server creates a separate thread responsible for receiving and processing messages from that client.

```text
                    Server
                       |
          +------------+------------+
          |            |            |
       Thread 1     Thread 2     Thread 3
          |            |            |
       Client 1     Client 2     Client 3
```

Connected clients are stored in a shared collection. Because multiple client threads can access this collection at the same time, a mutex is used to synchronize access and prevent race conditions.

The client also uses multithreading. The main thread handles user input and sends messages to the server, while a second thread continuously waits for incoming messages.

## Server Workflow

```text
socket()
   |
bind()
   |
listen()
   |
accept()
   |
create client thread
   |
send() / recv()
   |
close()
```

## Client Workflow

```text
socket()
   |
connect()
   |
send() / recv()
   |
close()
```

## Building the Project

Compile the server:

```bash
g++ -std=c++17 server.cpp -o server -pthread
```

Compile the client:

```bash
g++ -std=c++17 client.cpp -o client -pthread
```

If a `Makefile` is included, the project can also be compiled with:

```bash
make
```

## Running the Project

Start the server first:

```bash
./server
```

Then open another terminal and start a client:

```bash
./client
```

Open additional terminals to connect more clients:

```bash
./client
```

By default, the client connects to:

```text
127.0.0.1:54000
```

## Commands

### View connected users

```text
/users
```

### Disconnect from the server

```text
/quit
```

## Systems Concepts Practiced

### TCP Sockets

The project uses Linux socket APIs including:

- `socket()`
- `bind()`
- `listen()`
- `accept()`
- `connect()`
- `send()`
- `recv()`
- `close()`

These functions provide the communication layer between the server and clients.

### Multithreading

Each client connection is handled concurrently using `std::thread`.

This allows the server to continue communicating with other clients even when one client is waiting for input or network data.

### Synchronization

Multiple threads share the connected-client collection.

A `std::mutex` protects this shared resource, while `std::lock_guard` ensures that the mutex is safely released when the protected operation finishes.

### Race Conditions

Without synchronization, multiple threads could attempt to modify the connected-client collection at the same time.

Using a mutex prevents these conflicting operations from occurring simultaneously.

### Client-Server Architecture

The project separates responsibilities between:

- a central server that manages connections and communication
- clients that send and receive messages through the server

## Planned Improvements

Future improvements may include:

- Private messaging
- Chat rooms
- Message timestamps
- Server-side logging
- Improved command handling
- Graceful server shutdown using Linux signals
- Better socket error handling
- Configurable server IP and port
- Thread pool implementation
- `std::condition_variable`
- Synchronized task queue
- Automated tests

## Learning Goal

The goal of this project is to gain practical experience with C++ systems programming and better understand how networking, concurrency, synchronization, and operating-system APIs work together in a real client-server application.
