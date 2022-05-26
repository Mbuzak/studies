#include <iostream>
#include <cctype>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>

#define USED -1
#define INVALID -2

#define BLACK 0
#define RED 1
#define BLUE 2

#define HOST_IP "127.0.0.1"
#define SERVER_PORT 5001

using namespace std;



struct edge {
	char v1;
	char v2;
	int color;
};

edge *edges = new edge[15];
char letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};

void initial_edges() {
	int p = 0;
	for ( int i = 0 ; i < 6 ; i++ ) {
		for ( int j = i + 1 ; j < 6 ; j++ ) {
			edges[p].v1 = letters[i];
			edges[p].v2 = letters[j];
			edges[p].color = BLACK;
			p++;
		}
	}
}

void show_edges() {
	for ( int i = 0 ; i < 15 ; i++ ) {
		cout << edges[i].v1 << " - " << edges[i].v2 << " ";
		if ( edges[i].color == BLUE ) cout << "BLUE";
		else if ( edges[i].color == RED ) cout << "RED";
		cout << endl;
	}
}


int make_move(string vertices) {
	for ( int i = 0 ; i < 15 ; i++ ) {
		if ( (edges[i].v1 == vertices[0] and edges[i].v2 == vertices[1]) or
		     (edges[i].v1 == vertices[1] and edges[i].v2 == vertices[0]) ) {
			if ( edges[i].color != BLACK )
				return USED;
			return i;
		}
	}
	return INVALID;
}

bool check_input(string input) {
	if ( input.length() != 2 ) return false;
	return true;
}

// 1800 iterations
void scoreboard() {
	int triangle_count[] = {0, 0};
	for ( int i = 0 ; i < 6 - 2 ; i++ ) {
		for ( int j = i + 1 ; j < 6 - 1 ; j++ ) {
			for ( int k = j + 1 ; k < 6 ; k++ ) {
				int edges_count[] = {0, 0};  // [red, blue]
				for ( int q = 0 ; q < 15 ; q++ ) {
					// no need for reverse check because in edges struct first vertice is always lower than second
					if ( ( edges[q].v1 == letters[i] and edges[q].v2 == letters[j] ) or
					     ( edges[q].v1 == letters[j] and edges[q].v2 == letters[k] ) or
					     ( edges[q].v1 == letters[i] and edges[q].v2 == letters[k] ) ) {
						edges_count[edges[q].color - 1]++;
					}
				}
				
				if ( edges_count[0] == 3 ) triangle_count[0]++;
				else if ( edges_count[1] == 3 ) triangle_count[1]++;
			}
		}
	}
	
	cout << "Wynik czerwonego gracza: " << triangle_count[0] << endl;
	cout << "Wynik niebieskiego gracza: " << triangle_count[1] << endl;
}


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
	char *vertices = new char[2];
	int position;
	int remained_edges = 15;
	//char *buffer_client1, *buffer_client2;
	//int buffer_client1;
	char buffer_client1[1024], buffer_client2[1024];
	int players_color[] = {RED, BLUE};
	
	
	if ( recvfrom(socket_id, buffer_client1, 1024, 0,
		 (struct sockaddr *) &client1, &client_len ) < 0 ) {
		perror("recvfrom client 1 error");
		exit(0);
	}
	
	if ( recvfrom(socket_id, buffer_client2, 1024, 0,
		 (struct sockaddr *) &client2, &client_len ) < 0 ) {
		perror("recvfrom client 1 error");
		exit(0);
	}
	
	cout << "Game is starting" << endl;
	
	
	// sending colors
	strcpy(buffer_client1, "red");
	strcpy(buffer_client2, "blue");
	
	if ( sendto(socket_id, buffer_client1, 1024, 0,
			   (struct sockaddr *) &client1, client_len) < 0 ) {
		perror("sendto");
		exit(0);
	}
	cout << "Send message to client 1" << endl;

	if ( sendto(socket_id, buffer_client2, 1024, 0,
			   (struct sockaddr *) &client2, client_len) < 0 ) {
		perror("sendto");
		exit(0);
	}
	cout << "Send message to client 2" << endl;
	
	initial_edges();
	
	sleep(1);
	
	
	while ( running ) {
		// player 1
		if ( sendto(socket_id, edges, 1024, 0,
				   (struct sockaddr *) &client1, client_len) < 0 ) {
			perror("sendto");
			exit(0);
		}
		
		
		if ( recvfrom(socket_id, buffer_client1, 1024, 0,
			 (struct sockaddr *) &client1, &client_len ) < 0 ) {
			perror("recvfrom client 1 error");
			exit(0);
		}
		
		if ( make_move(buffer_client1) >= 0 ) {
			edges[position].color = RED;
			remained_edges--;
		}
		
		if ( remained_edges <= 0 ) {
			scoreboard();
			running = false;
		}
		
		// player 2
		if ( sendto(socket_id, edges, 1024, 0,
				   (struct sockaddr *) &client2, client_len) < 0 ) {
			perror("sendto");
			exit(0);
		}
		
		
		if ( recvfrom(socket_id, buffer_client2, 1024, 0,
			 (struct sockaddr *) &client2, &client_len ) < 0 ) {
			perror("recvfrom client 1 error");
			exit(0);
		}
		
		if ( make_move(buffer_client2) >= 0 ) {
			edges[position].color = BLUE;
			remained_edges--;
		}
		
		if ( remained_edges <= 0 ) {
			scoreboard();
			running = false;
		}
	}
	
	return 0;
}
