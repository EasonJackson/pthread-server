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
    Create an IPv4 socket and return a file descriptor, 
    
    ```int socket_fd = socket(AF_INET, SOCK_STREAM, 0)```
    
    With clarifications on the parameter:
    
    PARAMS | REPRESENTING
    --- | ---
    AF_INET | IPv4 
    SOCT_STEAM | stream-based
    0 | protocal likely set to TCP
    
2. Configuration
    
    Clear server address:
    
    ```bzero(&server_address, sizeof(server_address)```
    
    Socket level optional configuration, can be found [here](http://www.delorie.com/gnu/docs/glibc/libc_352.html)
    
    ```void setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, sizeof(set_reuse_addr))```
    
    Set Addressing scheme to – AF_INET ( IP )
    Allow any IP to connect – htons(INADDR_ANY)
    Listen on port SERVER_PORT – htons(SERVER_PORT)
    
    ```
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);
    ```
    
3. Binding
    
    Bind the file descriptor from server side to the server's address.
    
    ```bind(socket_fd, (struct sockaddr *) &server_address, sizeof(server_address))```

4. Listening on Port
    
    Let the file descriptor listen on the connections to the binded socket. ```max_pending_connections``` determines how many connections can be listened. When the number of connections is larger than max pending connection, create a new connection will result in failure.
    
    ```listen(socket_fd, max_pending_connections)```
    

5. Accept a Connection
    
    Accept a client socket and return a file descriptor associated with the socket.

    ```client_socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_address, &client_addr_len)```

### Communitating between Server and Client

1. An echo method of testing the connection between server and client.

    The client end reads a line of input string at each time and send it to server. The server will listen on the incoming message, output an handling message on it own console, and then responds the same message back to the client.

## Author
* **EasonJackson - @2018**
