//
//  main.c
//  OnlineChatServer
//
//  Created by EasonJackson on 3/14/18.
//  Copyright © 2018 EasonJackson. All rights reserved.
//

# include <stdio.h>
# include <pthread.h>
# include <netdb.h>

# define SERVER_ADDRESS "localhost"
# define SERVER_PORT 8080
# define NUMBER_OF_THREADS 2
# define max_pending_connections 2

void* read_from_client() {
    return NULL;
}

void* write_to_client() {
    return NULL;
}

int main(int argc, const char * argv[]) {
    
    char sendmessage[256];
    char recvmessage[256];
    pthread_t thread_pool[NUMBER_OF_THREADS];
    int thread_arg[NUMBER_OF_THREADS];
    int result;
    int server_socket_fd = 0, client_socket_fd = 0;
    int set_reuse_addr = 1;
    
    // Read thread
    thread_arg[0] = 0;
    result = pthread_create(&thread_pool[0], NULL, read_from_client, &thread_arg[0]);
    // Write thread
    thread_arg[1] = 1;
    result = pthread_create(&thread_pool[1], NULL, write_to_client, &thread_arg[1]);
    
    // Struct type to hold ip address and port
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    
    
    // 1.SOCKET CREATION
    // Create a socket with AF_INET(ip addressing) and of type SOCT_STREAM
    // Data from all devices wishing to connect to this socket will be redirected to listen_fd
    if (0 > (server_socket_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        fprintf(stderr, "Failed to create listening socket\n");
        exit(1);
    }
    
    // 2. SET SOCKET OPTIONS
    if (0 != setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &set_reuse_addr, sizeof(set_reuse_addr))) {
        fprintf(stderr, "server failed to set SO_REUSEADDR socket option (not fatal)\n");
    }
    // Clear address
    bzero(&server_address, sizeof(server_address));
    // Set Addressing scheme to – AF_INET ( IP )
    // Allow any IP to connect – htons(INADDR_ANY)
    // Listen on port 22000 – htons(22000)
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htons(INADDR_ANY);
    server_address.sin_port = htons(22000);
    
    // 3. BIND
    //Prepare to listen for connections from address/port specified in sockaddr ( Any IP on port 22000 )
    if (0 > (bind(server_socket_fd, (struct sockaddr *) &server_address, sizeof(server_address)))) {
        fprintf(stderr, "Fail to bind socket\n");
        exit(1);
    }
    
    // 4. LISTEN
    // Listen on the connections, with a connections limit of 1
    if (0 > listen(server_socket_fd, max_pending_connections)) {
        
    }
    
    // 5. ACCEPT
    if (0 > (client_socket_fd = accept(server_socket_fd, (struct sockaddr*) NULL, NULL))) {
        fprintf(stderr, "Server accepting failed");
    }
    
    while(1)
    {
        bzero(sendmessage, 256);
        bzero(recvmessage, 256);
        
    }
    
    close(server_socket_fd);
    return 0;
}
