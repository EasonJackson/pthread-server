# Concurrent socket chat - c posix-thread

A simple multithreaded chat room server and client program implemented with c p-thread and socket.

## Introduction

## Getting Started

### Installation

Download source code to local directory. Compile with IDE that works with C, or use gcc lib tool.

### Running Server Program

Under dir ```/OnlineChatServer```

```
gcc -o ChatServer main.c
```

Run the executable ```ChatServer```, which runs on ```localhost:8080```

```
Server socket created...
Type: IPv4
Stream-based socket
Bind socket successfully...
Server listening for a connection on port 8080
```

### Running Client Program


Under dir ```/OnlineChatClient```

```
gcc -o ChatClient main.c
```

### C Socket
1. Socket Creation
    Create an IPv4 socket, ```server_socket_fd = socket(AF_INET, SOCK_STREAM, 0)```
    
    With clarifications on the parameter:
    
    AF_INET: IPv4
    
    SOCT_STREAM: stream-based
    
    0: protocal likely set to TCP
    
2. Configuration
3. Binding
4. Listening on Port
5. Accept a Connection

### Communitating between Server and Client

Both server and client use two different posix threads, one for read message task and the other for write message task.

## Author
* **EasonJackson - @2018**
