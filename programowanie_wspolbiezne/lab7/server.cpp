#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <signal.h>


#define HOST_IP "127.0.0.1"
#define SERVER_PORT 5051

#define ROCK 1
#define PAPER 2
#define	SCISSORS 3

#define DRAW 0
#define WIN  1
#define LOSS -1

using namespace std;


void clearup (int sig) {
    exit(0);
}

int main(int argc, char *argv[]) {
	signal(SIGINT, clearup);
	
    int socket_id, buffer_client1, buffer_client2;
    socklen_t client_address_len = sizeof (struct sockaddr_in);

    struct sockaddr_in server_address, client_address_1, client_address_2;

	// UDP socket
    if ( (socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket");
        exit(1);
    }

	//
    server_address.sin_family = AF_INET;
    if ( inet_pton(AF_INET, HOST_IP, &server_address.sin_addr ) <= 0 ) {
        perror("inet_pton");
        exit(1);
    }

	//
    server_address.sin_port = htons ((unsigned short) SERVER_PORT);
    if ( bind(socket_id, (struct sockaddr *) &server_address, sizeof (server_address)) < 0 ) {
		perror("bind");
        exit(1);
    }

	bool running = true;
    while ( running )
    {
        if ( recvfrom(socket_id, (char *) &buffer_client1, sizeof (int), 0,
                      (struct sockaddr *) &client_address_1, &client_address_len) < 0 ) {
            perror("recvfrom");
            exit(1);
        }

        if ( recvfrom(socket_id, (char *) &buffer_client2, sizeof (int), 0,
                      (struct sockaddr *) &client_address_2, &client_address_len) < 0 ) {
			perror("recvfrom");
            exit(1);
        }
        
		// conversion
        buffer_client1 = htonl(buffer_client1);
        buffer_client2 = htonl(buffer_client2);
        

        if ( buffer_client1 == buffer_client2 ) {
            buffer_client1 = buffer_client2 = DRAW;
        } else if ( (buffer_client1 == ROCK && buffer_client2 == PAPER) ||
					(buffer_client1 == SCISSORS && buffer_client2 == ROCK) ||
					(buffer_client1 == PAPER && buffer_client2 == SCISSORS) ) {
            buffer_client1 = LOSS;
            buffer_client2 = WIN;
        } else if ( (buffer_client1 == ROCK && buffer_client2 == SCISSORS) ||
					(buffer_client1 == SCISSORS && buffer_client2 == PAPER) || 
					(buffer_client1 == PAPER && buffer_client2 == ROCK) ) {
            buffer_client1 = WIN;
            buffer_client2 = LOSS;
        }

        if ( sendto(socket_id, (char *) &buffer_client1, sizeof(int), 0,
                   (struct sockaddr *) &client_address_1, client_address_len) < 0 ) {
            perror("sendto");
            exit(1);
        }

        if ( sendto(socket_id, (char *) &buffer_client2, sizeof(int), 0,
                   (struct sockaddr *) &client_address_2, client_address_len) < 0 ) {
            perror("sendto");
            exit(1);
        }
    }
	
	return 0;
}
