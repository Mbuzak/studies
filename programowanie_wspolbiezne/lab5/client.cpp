#include <iostream>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
//#include <stdlib.h>

#define CLIENT_KEY 500
#define SERVER_KEY 400
#define MAX_SIZE 50

using namespace std;

struct message {
	long int type;
	char text[MAX_SIZE];
	
};

int main(int argc, char *argv[]) {
	int msgid_client;
	int msgid_server;
	int pid;
	
	
	msgid_client = msgget(CLIENT_KEY, 0600);
	msgid_server = msgget(SERVER_KEY, 0600);
	
	
	if ( msgid_client == -1 || msgid_server == -1) {
		cout << "Błąd tworzenia kolejki" << endl;
		exit(1);
	}
	
	pid = getpid();
	
	char input[MAX_SIZE];
	cout << "Podaj tekst: ";
	cin >> input;
	
	message msg;
	strcpy(msg.text, input);
	// msg.type = 1;
	msg.type = pid;
	
	if ( msgsnd(msgid_server, &msg, MAX_SIZE, 0) == -1 ) {
		cout << "Błąd wysłania wiadomości" << endl;
		exit(1);
	}
	
	cout << "Wysłano wiadomość" << endl;
	sleep(3);
	cout << "Oczekiwanie na odpowiedź.." << endl << endl;
	sleep(3);
	
	if ( msgrcv(msgid_client, &msg, MAX_SIZE, 0, 0) == -1) {
		cout << "error" << endl;
		exit(1);
	}
	
	cout << "Przetłumaczone słowo: " << msg.text << endl;
	
	
	return 0;
}
