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
	
	// disable buffering
	setbuf(stdout, NULL);
 	setbuf(stderr, NULL);


	struct sockaddr_in serv_addr;
	int server_fd;
	
	// fire up server file descriptor and struct
	server_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (server_fd == -1) { // oops!
		perror("Socket failed.");
        close(server_fd);
        exit(EXIT_FAILURE);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

	// setting socket options to avoid "already in use" errors
	int reuse = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		perror("Setting SO_REUSEADDR failed.");
        close(server_fd);
        exit(EXIT_FAILURE);
	}

	// must bind() to a port on the machine so that server can listen()
	if (bind(server_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) != 0) {
		perror("Bind failed.");
        close(server_fd);
        exit(EXIT_FAILURE);
	}

	// begin listening w/ backlog of 10
	if (listen(server_fd, 10) != 0) {
		perror("Listening failed.");
        close(server_fd);
        exit(EXIT_FAILURE);
	} // success!
	
	
	printf("Waiting for a client to connect...\n");

	while(1){
		
		struct sockaddr_in client_addr;
		socklen_t client_addr_len = sizeof(client_addr);

		int *client_fd = malloc(sizeof(int)); // pointer allows fd to be passed within threads

		*client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
		if(*client_fd < 0){
			perror("Accept failed");
    		free(client_fd);
   	 		continue;
		}	

		printf("Client connected\n"); //yas

		handle_client(client_fd);
		close(client_fd);

	}

	//final steps
	close(server_fd);

	return 0;
}


void *handle_client(int client_fd) {
	*char response = "HTTP/1.1 404 Not Found\r\n\r\n";
	char request[BUFFER_SIZE];

	int bytes_received = recv(client_fd, request, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0) {
        perror("Receive failed");
        return;
    }
	request[BUFFER_SIZE - 1] = '\0';

	*char first = strtok(request, ' ');
	*char sec = strtok(request,' ');
	if(strcmp(sec, '/') == 0){
		response = 
	}


	
}



// Notes:

//ON SOCKET():
// IPv4 or IPv6, stream or datagram, TCP or UDP.
// socket() simply returns to you a socket descriptor 

//ON SETTING SOCKET OPTIONS:
// SO_REUSEADDR ensures that we don't run into 'Address already in use' errors
// Setting socket options for server_fd. will return neg if it fails.

