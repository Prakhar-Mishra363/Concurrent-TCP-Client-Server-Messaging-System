# Concurrent TCP Client-Server Messaging System

A multi-client TCP communication system developed in C using Linux socket programming and process-based concurrency. The server handles multiple clients simultaneously by creating separate child processes using `fork()`.

## Features

- **TCP Communication:** Establishes reliable client-server connections using TCP sockets.
- **Concurrent Client Handling:** Supports multiple simultaneous client connections using process creation with `fork()`.
- **Message Framing:** Implements length-prefixed message transmission to identify message boundaries.
- **Reliable Data Transfer:** Custom `sendAll()` and `recvAll()` functions handle partial data transmission and reception.
- **Process Management:** Uses `SIGCHLD` and `waitpid()` to detect and reap terminated child processes, preventing zombie processes.
- **Event Monitoring:** Uses `poll()` to monitor incoming connection requests.

## Project Structure

```text
.
├── client.c
├── server.c
├── sendAndRecvAll.c
├── sendAndRecvAll.h
└── README.md
```

## Technologies Used

- C Programming
- Linux System Programming
- TCP/IP Socket Programming
- Process Management (`fork`, `waitpid`)
- Signal Handling (`SIGCHLD`)
- I/O Multiplexing (`poll`)

## How It Works

1. The server creates a TCP socket and binds it to `127.0.0.1:8080`.
2. The server listens for incoming client connections.
3. When a client connects, the server accepts the connection and creates a child process using `fork()`.
4. The child process handles communication with that client independently.
5. Clients send messages to the server, which receives and responds to them.
6. Message lengths are transmitted before message data to maintain message boundaries.
7. The server monitors child process termination and reaps completed processes using `waitpid()`.

## Compilation

Compile the server:

```bash
gcc server.c sendAndRecvAll.c -o server
```

Compile the client:

```bash
gcc client.c sendAndRecvAll.c -o client
```

## Execution

**Terminal 1 – Start the server:**

```bash
./server
```

**Terminal 2 – Start a client:**

```bash
./client
```

Open additional terminals and execute `./client` to establish multiple simultaneous connections.

## Learning Outcomes

- Understanding TCP socket communication and client-server architecture.
- Working with Linux system calls for process creation and management.
- Handling partial reads and writes during network communication.
- Understanding message framing in TCP streams.
- Managing child processes and preventing zombie processes.
- Using `poll()` and signal handling for event-driven server management.

## Future Improvements

- Support communication between different machines over a network.
- Implement graceful server shutdown.
- Add support for multiple messages and improved connection error handling.
