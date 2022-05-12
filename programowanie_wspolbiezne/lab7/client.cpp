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

#define WIN  1
#define DRAW 0
#define LOSS -1


using namespace std;


int main(int argc, char *argv[]) {
	int wins = 0, losses = 0;
    
    int socket_id, buffer = 0;
    socklen_t server_address_len;

    struct sockaddr_in server_address, client_address;

	// UDP socket
    if ( (socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket");
        exit(1);
    }

    if ( inet_pton (AF_INET, HOST_IP, &server_address.sin_addr) <= 0 ) {
        perror("inet_pton");
        exit(1);
    }

    server_address.sin_port = htons((unsigned short) SERVER_PORT);
    server_address_len = sizeof(server_address);
    
    client_address.sin_family = AF_INET;
    client_address.sin_port = htons(0);

	bool running = true;
    while (running)
    {
		cout << "rock(1)   paper(2)   scissors(3)" << endl;
		cin >> buffer;

        buffer = ntohl(buffer);

        if ( sendto(socket_id, (char *) &buffer, sizeof(int), 0,
                   (struct sockaddr *) &server_address, server_address_len) < 0 ) {
            perror("sendto");
            exit(1);
        }
        cout << "Waiting for results.." << endl;

        if ( recvfrom(socket_id, (char *) &buffer, sizeof(int), 0,
                      (struct sockaddr *) &server_address, &server_address_len) < 0 ) {
            perror("recvfrom");
            exit(1);
        }

        switch (buffer) {
			case WIN:
                wins++;
                cout << "Win: " << wins << " - " << losses << endl;
                break;
            case LOSS:
                losses++;
                cout << "Lose: " << wins << " - " << losses << endl;
                break;
            case DRAW:
                cout << "Draw: " << wins << " - " << losses << endl;
                break;        
            default:
				perror("Invalid data received from server");
                exit(1);
                break;
        }
        
        if ( wins == 2 || losses == 2 ) {
			running = false;
		}
    }

    if ( wins > losses )
        cout << "You win the game!" << endl;
    else
        cout << "You lose the game" << endl;
	
	return 0;
}
