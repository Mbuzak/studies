#include <unistd.h>
#include "helper.hpp"

using namespace std;

edge *edges = new edge[15];


int main(int argc, char *argv[]) {
	int socket_id;
	socklen_t client_len = sizeof(struct sockaddr_in);
	struct sockaddr_in server, client1, client2;
	
	if ( (socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket error");
		exit(0);
	}
	cout << "Successful socket operation" << endl;
	
	server.sin_family = AF_INET;
	server.sin_port = htons(SERVER_PORT);
	
	if ( inet_pton(AF_INET, HOST_IP, &server.sin_addr ) < 1 ) {
		perror("inet_pton error");
		exit(0);
	}
	cout << "Successful inet operation" << endl;
	
	if ( bind(socket_id, (struct sockaddr *) &server, sizeof(server) ) < 0 ) {
		perror("bind socket error");
		exit(0);
	}
	cout << "Successful bind socket address" << endl;



	bool running = true;
	int position;
	int remained_edges = 15;
	char buffer_client1[10], buffer_client2[10];
	
	
	if ( recvfrom(socket_id, buffer_client1, 10, 0,
		 (struct sockaddr *) &client1, &client_len ) < 0 ) {
		perror("recvfrom client 1 error");
		exit(0);
	}
	
	if ( recvfrom(socket_id, buffer_client2, 10, 0,
		 (struct sockaddr *) &client2, &client_len ) < 0 ) {
		perror("recvfrom client 1 error");
		exit(0);
	}
	
	cout << "Game is starting" << endl;
	
	
	// sending colors
	strcpy(buffer_client1, "red");
	strcpy(buffer_client2, "blue");
	
	if ( sendto(socket_id, buffer_client1, 10, 0,
			   (struct sockaddr *) &client1, client_len) < 0 ) {
		perror("sendto");
		exit(0);
	}
	cout << "Send message to client 1" << endl;

	if ( sendto(socket_id, buffer_client2, 10, 0,
			   (struct sockaddr *) &client2, client_len) < 0 ) {
		perror("sendto");
		exit(0);
	}
	cout << "Send message to client 2" << endl;
	
	initial_edges(edges);
	
	sleep(1);
	
	
	while ( running ) {
		// player 1
		if ( sendto(socket_id, (edge *) edges, sizeof(edges)*15, 0,
				   (struct sockaddr *) &client1, client_len) < 0 ) {
			perror("sendto");
			exit(0);
		}
		
		if ( recvfrom(socket_id, buffer_client1, 10, 0,
			 (struct sockaddr *) &client1, &client_len ) < 0 ) {
			perror("recvfrom client 1 error");
			exit(0);
		}
		
		position = make_move(edges, buffer_client1);
		if ( position >= 0 ) {
			edges[position].color = RED;
			remained_edges--;
		}
		
		if ( remained_edges <= 0 ) {
			remained_edges = 15;
			reset_edges(edges);
			continue;
		}
		
		// player 2
		if ( sendto(socket_id, (edge *) edges, sizeof(edges)*15, 0,
				   (struct sockaddr *) &client2, client_len) < 0 ) {
			perror("sendto");
			exit(0);
		}
		
		if ( recvfrom(socket_id, buffer_client2, 10, 0,
			 (struct sockaddr *) &client2, &client_len ) < 0 ) {
			perror("recvfrom client 1 error");
			exit(0);
		}
		
		position = make_move(edges, buffer_client2);
		if ( position >= 0 ) {
			edges[position].color = BLUE;
			remained_edges--;
		}
		
		if ( remained_edges <= 0 ) {
			remained_edges = 15;
			reset_edges(edges);
			continue;
		}
	}
	
	delete edges;
	
	return 0;
}
