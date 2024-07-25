#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define PORT 4221
#define BUFFER_SIZE 1024
void handle_client(int client_fd);

int main() {
	// Disable output buffering, means that output to these streams is written immediately, 
	//rather than being buffered (stored temporarily) and written in larger chunks.
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);

	


	// socket(): Creates a socket for communication (server_fd).
	// bind(): Binds the socket to the server address (INADDR_ANY and port 4221).
	// listen(): Listens for incoming connections on the server socket.
	// accept(): Accepts an incoming connection, creating a new socket (client_socket) for communication with the client.

	int server_fd; //server socket file descriptor
	socklen_t client_addr_len; //holds the size of the client address structure.
	struct sockaddr_in client_addr;
	
	server_fd = socket(AF_INET, SOCK_STREAM, 0); //socket(domain, type, protocol) aka IPv4 or IPv6, stream or datagram, and TCP or UDP).
	//socket() simply returns to you a socket descriptor 
	if (server_fd == -1) {
		printf("Socket creation failed: %s...\n", strerror(errno));
		return 1;
	}
	
	// Since the tester restarts your program quite often, setting SO_REUSEADDR
	// ensures that we don't run into 'Address already in use' errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		printf("SO_REUSEADDR failed: %s \n", strerror(errno));
		return 1;
	}
	
	struct sockaddr_in serv_addr = { .sin_family = AF_INET ,
									 .sin_port = htons(4221),
									 .sin_addr = { htonl(INADDR_ANY) },
									}; // we're hardcoding this instead of using getaddrinfo()

	//must bind() to a port on the machine so that server can listen()
	
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		printf("Bind failed: %s \n", strerror(errno));
		return 1;
	}
	
	int connection_backlog = 5;
	if (listen(server_fd, connection_backlog) != 0) {
		printf("Listen failed: %s \n", strerror(errno));
		return 1;
	}
	
	printf("Waiting for a client to connect...\n");
	client_addr_len = sizeof(client_addr);
	
	//accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len); <-- this was the OG
	//client_socket = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);

	int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len); // this is a SECOND file descriptor for this particular connection.
	if (client_fd == -1) {
        printf("Accept failed: %s\n", strerror(errno));
        close(server_fd);
        return 1;
    }
	// the other file descriptor is still listening for connections.


	printf("Client connected\n"); //yas

	//char *reply = "HTTP/1.1 200 OK\r\n\r\n";
  	//int bytes_sent = send(client_fd, reply, strlen(reply), 0); //Finally, the server sends this response to the client using the send function
	//int send(int sockfd, const void *msg, int len, int flags); 
	
	handle_client(client_fd);


	close(client_fd);
	close(server_fd); // we always close what we open to avoid valgrind errors

	return 0;
}


void handle_client(int client_fd){
 char req[BUFFER_SIZE] = {'\0'}; // Initialize the request buffer to all null characters
    char sep[] = " "; // sep should be a string
    int bytes_received = recv(client_fd, req, BUFFER_SIZE - 1, 0); //recv() function is used in socket programming to receive data from a socket
	//int sockfd, void *buf, size_t len, int flags
    if (bytes_received < 0) { // checking for error
        printf("Receive failed: %s\n", strerror(errno));
        return;
    }

    printf("Received request:\n%s\n", req); // yay!
    int j; // this will be used as a counter
    char *str_token, *resp = "HTTP/1.1 404 Not Found\r\n\r\n"; // Default response
	// str_token is a pointer to store tokens while parsing response
	// resp is a pointer to a string that stores the default HTTP response, which is "404 Not Found"
    for (j = 0, str_token = strtok(req, sep); str_token; j++, str_token = strtok(NULL, sep)) { 
		//This loop uses strtok to tokenize the request using space as a delimiter.
        if (j == 1) { // When j equals 1 (i.e., the second token, which is the requested path), it checks the value of str_token.
            printf("Requested path: %s\n", str_token); // Debugging info
            if (strcmp(str_token, "/") == 0) { //If the requested path is /, the response is set to "200 OK".
                resp = "HTTP/1.1 200 OK\r\n\r\n";
            } else {
                resp = "HTTP/1.1 404 Not Found\r\n\r\n";
            }
            break; // No need to continue parsing
        }
    }
    int resp_len = strlen(resp);
    printf("Sending: %s\n", resp);
    int bytes_sent = send(client_fd, resp, resp_len, 0);
    if (bytes_sent < 0) {
        printf("Send failed: %s\n", strerror(errno));
    }
}

