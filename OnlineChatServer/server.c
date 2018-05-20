//
//  server.c
//  OnlineChatServer
//
//  Created by EasonJackson on 3/14/18.
//  Copyright © 2018 EasonJackson. All rights reserved.
//

# include <stdio.h>
# include <pthread.h>
# include <netdb.h>
# include <arpa/inet.h>
# include <ctype.h>
# include <netdb.h>
# include <netinet/in.h>
# include <stdlib.h>
# include <string.h>
# include <sys/socket.h>
# include <unistd.h>

# define SERVER_ADDRESS "localhost"
# define SERVER_PORT 8080
# define max_pending_connections 2

// Send and receive message buffer
// Buffer size = 1024 char
static const size_t BUFFER_SIZE = 1024;
static char sendmessage[BUFFER_SIZE];
static char recvmessage[BUFFER_SIZE];

pthread_mutex_t gSharedMemoryLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gReadPhase = PTHREAD_COND_INITIALIZER;

// Thread handler func declearations
void* handle_client(void*);

int serve(int argc, const char * argv[]) {
    pthread_t thread;
    
    int server_socket_fd = 0, client_socket_fd = 0;
    int set_reuse_addr = 1; // set reuse address ON = 1
    
    // Struct type to hold ip address and port
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    struct hostent* client_host_info;
    char* client_host_ip;
    socklen_t client_addr_len;
    
    // 1.SOCKET CREATION
    // Create a socket with AF_INET(ip addressing) and of type SOCT_STREAM
    // AF_INET: IPv4
    // SOCT_STREAM: stream-based
    // 0: protocal likely set to TCP
    // Data from all devices wishing to connect to this socket will be redirected to listen_fd
    if (0 > (server_socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        fprintf(stderr, "Failed to create listening socket\n");
        exit(1);
    } else {
        fprintf(stdout, "Server socket created...\n");
        fprintf(stdout, "Type: IPv4\n");
        fprintf(stdout, "Stream-based socket\n");
    }
    
    // 2. SET SOCKET OPTIONS
    if (0 != setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, sizeof(set_reuse_addr))) {
        fprintf(stderr, "server failed to set SO_REUSEADDR socket option (not fatal)\n");
    }
    // Clear address
    bzero(&server_address, sizeof(server_address));
    bzero(&client_address, sizeof(client_address));
    // Set Addressing scheme to – AF_INET ( IP )
    // Allow any IP to connect – htons(INADDR_ANY)
    // Listen on port
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(SERVER_PORT);
    
    // 3. BINDING
    //Prepare to listen for connections from address/port specified in sockaddr ( Any IP on port 22000 )
    if (0 > (bind(server_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)))) {
        fprintf(stderr, "Fail to bind socket\n");
        exit(1);
    } else {
        fprintf(stdout, "Bind socket successfully...\n");
    }
    
    // 4. LISTEN
    // Listen on the connections
    if (0 > listen(server_socket_fd, max_pending_connections)) {
        fprintf(stderr, "Listening on socket error");
        exit(1);
    } else {
        fprintf(stdout, "Server listening for a connection on port %d\n", SERVER_PORT);
    }
    
    // Get the size client's address structure
    client_addr_len = sizeof(client_address);
    
    // 5. ACCEPT
    while (1) {
        if (0 > (client_socket_fd = accept(server_socket_fd, (struct sockaddr*) &client_address, &client_addr_len))) {
            fprintf(stderr, "Server accepting failed.\n");
        } else {
            fprintf(stdout, "Server accepted a client.\n");
            client_host_info = gethostbyaddr((const char*)&client_address.sin_addr.s_addr,
                                             sizeof(client_address.sin_addr.s_addr),
                                             AF_INET);
            if (client_host_info == NULL) {
                fprintf(stderr, "Server could not determine client host address\n");
            }
            client_host_ip = inet_ntoa(client_address.sin_addr);
            if (client_host_ip == NULL) {
                fprintf(stderr, "Server could not determine client host ip\n");
            }
            fprintf(stdout, "Server established connection with %s (%s)\n", client_host_info->h_name, client_host_ip);
            
            // Create thread
            if (pthread_create(&thread, NULL, handle_client, (void*) &client_socket_fd) != 0) {
                fprintf(stderr, "Failed to create thread\n");
                exit(1);
            }
        }
    }

    pthread_join(thread, NULL);
    
    return 0;
}

// Definition of handle_client function
void* handle_client(void* client_socket_fd) {
    int* client_socket_fd_p = (int*) client_socket_fd;
    fprintf(stdout, "Created thread...\n");
    write(*client_socket_fd_p, "Welcome client.\n", 15);
    
    while(1) {
        // Clear the reader buffer
        bzero(recvmessage, BUFFER_SIZE);
        
        // Read info from client socket
        ssize_t num_bytes = read(*client_socket_fd_p, recvmessage, BUFFER_SIZE);
        
        // Output error if there is an error;
        // Output client address + message in the buffer.
        if (num_bytes == 0) {
            fprintf(stderr, "Server cannot read from client");
        } else {
            fprintf(stdout, "Message from client %d: %s", *client_socket_fd_p, recvmessage);
            // Echo message to client
            write(*client_socket_fd_p, recvmessage, strlen(recvmessage));
        }
    }
    
    return NULL;
}
