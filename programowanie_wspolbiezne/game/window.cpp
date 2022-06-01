#include "helper.hpp"
#include "GUI_helper.hpp"

using namespace std;


edge *edges = new edge[15];


int main(int argc, char *argv[]) {
	// INITIAL UDP
	int socket_id;
	socklen_t server_len = sizeof(struct sockaddr_in);
    struct sockaddr_in server;
    
    if ( (socket_id = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket error");
        exit(0);
    }
    
    server.sin_port = htons(SERVER_PORT);
    server.sin_family = AF_INET;
    
    if ( inet_pton (AF_INET, HOST_IP, &server.sin_addr) < 1 ) {
        perror("inet_pton error");
        exit(1);
    }
	
	int color;
	bool running = true;
	char *buffer = new char[10];
	int position;
	string str;
	
	strcpy(buffer, "ready");
	if ( sendto(socket_id, buffer, 10, 0,
		 (struct sockaddr *) &server, sizeof(server)) < 0 ) {
		perror("sendto error");
		exit(0);
	}
	cout << "Dołączanie.." << endl;
	
	
	if ( recvfrom(socket_id, buffer, 10, 0,
	     (struct sockaddr *) &server, &server_len ) < 0 ) {
		perror("recvfrom");
		exit(0);
	}
	
	// cout << buffer << endl;
	if ( strcmp(buffer, "red") == 0 ) {
		color = RED;
		cout << "GRACZ CZERWONY" << endl;
	}
	else {
		color = BLUE;
		cout << "GRACZ NIEBIESKI" << endl;
	}
	
	
	initial_edges(edges);
	init_vertices();
	reset_line(line);
	init(color);
	
	
	// GAME LOOP
	while ( running ) {
		cout << "Tura 2 gracza..." << endl;
		
		if ( recvfrom(socket_id, (edge *) edges, sizeof(edges)*15, 0, (struct sockaddr *) &server,
			 &server_len ) < 0 ) {
			perror("recvfrom");
			exit(0);
		}
		cout << endl << "Twoja tura!" << endl << endl;
		
		//show_edges(edges);
		
		while ( true ) {
			show_game(edges);
			
			/*if ( event.type == Expose && event.xexpose.count == 0 ) {
				draw();
			}*/
			
			if ( event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1 ) {
				if ( text[0] == 'q' ) {
					close();
				}
			}
			
			if ( event.type == ButtonPress ) {
				int x = event.xbutton.x, y = event.xbutton.y;
				
				char choice = clicked_letter(x, y);
				cout << "Litera:" << choice << endl;
				
				// before send message it have to has 2 LETTERS
				if ( choice != ' ' and line[0] == ' ' ) line[0] = choice;
				else if ( choice != ' ' and line[0] != ' ' ) line[1] = choice;
				
				// case when user double click same letter
				if ( line[0] == line[1] ) line[1] = ' ';
				
				// check invalid input
				if ( line[0] != ' ' and line[1] != ' ' and
				     make_move(edges, line) < 0 ) {
					cout << "Ta krawędź jest już zajęta. Wybierz inną" << endl;
					reset_line(line);
				}
				
				// send message
				if ( line[0] != ' ' and line[1] != ' ' ) {
					buffer = line;
					
					if ( sendto(socket_id, buffer, 10, 0,
						 (struct sockaddr *) &server, sizeof(server)) < 0 ) {
						perror("sendto error");
						exit(0);
					}
					
					// adding new edge
					position = make_move(edges, buffer);
					edges[position].color = color;
					
					reset_line(line);
					//XClearWindow(dis, win);
					break;
				}
				
			}
		
		}
		if ( empty_edges(edges) <= 1 ) {
			fill_last_edge(edges, color);
			scoreboard(edges);
			cout << "Czy chcesz zagrać ponownie? y/n";
			
			while ( true ) {
				show_game(edges);
				
				if ( event.type == KeyPress && XLookupString(&event.xkey, text, 255, &key, 0) == 1 ) {
					if ( text[0] == 'n' ) {
						close();
					}
					
					else if ( text[0] == 'y' ) {
						reset_edges(edges);
						XClearWindow(dis, win);
						break;
					}
				}
			}
		}
		
		cout << endl << "Następna runda" << endl;
		cout << "Aby koontunuować kliknij w dowolnym miejscu..." << endl;
		
		show_game(edges);
		continue;
	}
	
	
	return 0;
}
