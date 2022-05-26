#include <iostream>
#include <cctype>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>

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

int main(int argc, char *argv[]) {
	int socket_id;
	socklen_t server_len = sizeof(struct sockaddr_in);
    struct sockaddr_in server, client;
    
    if ( (socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket error");
        exit(0);
    }
    
    server.sin_port = htons(SERVER_PORT);
    server.sin_family = AF_INET;
    
    client.sin_family = AF_INET;
    client.sin_port = htons(0);
    
    if ( inet_pton (AF_INET, HOST_IP, &server.sin_addr) < 1 ) {
        perror("inet_pton error");
        exit(1);
    }
	
	
	
	int color;
	bool running = true;
	char buffer[1024];
	int position;
	
	strcpy(buffer, "ready");
	if ( sendto(socket_id, buffer, 1024, 0,
		 (struct sockaddr *) &server, sizeof(server)) < 0 ) {
		perror("sendto error");
		exit(0);
	}
	cout << "Dołączanie.." << endl;
	
	
	if ( recvfrom(socket_id, buffer, 1024, 0, (struct sockaddr *) &server,
		 &server_len ) < 0 ) {
		perror("recvfrom");
		exit(0);
	}
	
	cout << buffer << endl;
	if ( strcmp(buffer, "red") == 0 ) {
		color = RED;
		cout << "GRACZ CZERWONY" << endl;
	}
	else {
		color = BLUE;
		cout << "GRACZ NIEBIESKI" << endl;
	}
	
	
	
	while ( running ) {
		cout << "Tura 2 gracza" << endl;
		if ( recvfrom(socket_id, edges, 1024, 0, (struct sockaddr *) &server,
			 &server_len ) < 0 ) {
			perror("recvfrom");
			exit(0);
		}
		
		show_edges();
		
		cout << endl << "Twoja tura" << endl;
		
		// loop for valid user input
		while ( true ) {
			cout << "Podaj dwa wierzchołki: ";
			cin >> buffer;
			
			if ( !check_input(buffer) ) {
				continue;
			}
			
			char *capital = new char[2];
			capital[0] = toupper(buffer[0]);
			capital[1] = toupper(buffer[1]);
			
			position = make_move(capital);
			
			delete capital;
			
			if ( position >= 0 ) {
				edges[position].color = color;
				break;
			}
			else if ( position == USED ) cout << "Ta krawędź jest już zajęta. Proszę wybrać inną" << endl;
			else if ( position == INVALID ) cout << "Podano nieistniejącą krawędź. Spróbuj ponownie" << endl;
		}
		
		show_edges();
		cout << buffer << endl;
		strcpy(buffer, "ab");
		if ( sendto(socket_id, buffer, 1024, 0,
			 (struct sockaddr *) &server, sizeof(server)) < 0 ) {
			perror("sendto error");
			exit(0);
		}
		
	}
	
	return 0;
}
