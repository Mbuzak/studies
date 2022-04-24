//ipcrm --all=msg - usuwa kolejke ipcs

#include <iostream>
#include <string>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <string.h>

#define CLIENT_KEY 500
#define SERVER_KEY 400
#define MAX_SIZE 50


using namespace std;


struct message {
	long int type;
	char word[MAX_SIZE];
};


struct translation {
	char pol_word[MAX_SIZE];
	char eng_word[MAX_SIZE];
};


void initialize_database(translation t[]) {
	strcpy(t[0].pol_word, "Orzeł"); strcpy(t[0].eng_word, "Eagle");
	strcpy(t[1].pol_word, "Wróbel"); strcpy(t[1].eng_word, "Sparrow");
	strcpy(t[2].pol_word, "Wrona"); strcpy(t[2].eng_word, "Crow");
	strcpy(t[3].pol_word, "Kruk"); strcpy(t[3].eng_word, "Raven");
	strcpy(t[4].pol_word, "Gołąb"); strcpy(t[4].eng_word, "Pigeon");
}


char* translate(translation t[], size_t q, char* word) {
	for ( int i = 0; i < q; i++ ) {
		if ( strcmp(t[i].pol_word, word) == 0 ) {
			return t[i].eng_word;
		}
	}
	return (char*)"Nie znam takiego słowa";
}


void show_translations(translation t[], size_t q) {
	for ( int i = 0; i < q; i++ ) {
		cout << t[i].pol_word << "   " << t[i].eng_word << endl;
	}
}



int main(int argc, char *argv[]) {
	
	// signal(SIGINT, cleanup);
	
	size_t q = 5;
	translation *t = new translation[q];
	
	initialize_database(t);
	
	
	int msgid_client;
	int msgid_server;
	
	msgid_client = msgget(CLIENT_KEY, 0600 | IPC_CREAT);
	msgid_server = msgget(SERVER_KEY, 0600 | IPC_CREAT);
	
	cout << "Utworzono kolejke" << endl << endl;
	
	message msg;
	
	bool running = true;
	
	while ( running ) {
		if ( msgrcv(msgid_server, &msg, MAX_SIZE, 0, 0) == -1) {
			cout << "error" << endl;
		}
		
		strcpy(msg.word, translate(t, q, msg.word));
		
		if ( msgsnd(msgid_client, (void *)&msg, MAX_SIZE, 0) == -1 ) {
			cout << "Błąd wysłania odpowiedzi" << endl;
		}
		else {
			cout << "Wysłano odpowiedź..." << endl;
		}
		
		
		// running = false;
	}
	
	
	return 0;
}
