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

int main(){
    setbuf(stdout, NULL); 
    setbuf(stderr, NULL); // we're setting the destination of these file streams. in this case it's null.
    // why is it null? idk. best practice i suppose.

    socklen_t client_addr_len; //holds the size of the client address structure.
	struct sockaddr_in client_addr;

    int listener_fd = socket(AF_INET, SOCK_STREAM, 0); 
    //socket(domain, type, protocol) aka IPv4 or IPv6, stream or datagram, and TCP or UDP).

    if (listener_fd == -1) {
		printf("Socket creation failed");
		return 1;
	}

    int yes=1;
    // Via beej's: what will this do?
    if (setsockopt(listener_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof (yes)) == -1) {
        perror("setsockopt");
        exit(1);
    } 















}