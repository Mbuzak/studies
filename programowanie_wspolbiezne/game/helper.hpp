#include <iostream>
#include <cstring>
#include <cctype> // toupper
#include <sys/socket.h>
#include <arpa/inet.h>

#define USED -1
#define INVALID -2

#define BLACK 0
#define RED 1
#define BLUE 2

#define HOST_IP "127.0.0.1"
#define SERVER_PORT 5001

struct edge {
	char v1;
	char v2;
	int color;
};

using namespace std;

char letters[] = {'A', 'B', 'C', 'D', 'E', 'F'};

void initial_edges(edge *edges) {
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

void show_edges(edge *edges) {
	for ( int i = 0 ; i < 15 ; i++ ) {
		cout << edges[i].v1 << " - " << edges[i].v2 << " ";
		if ( edges[i].color == BLUE ) cout << "BLUE";
		else if ( edges[i].color == RED ) cout << "RED";
		cout << endl;
	}
}


int make_move(edge *edges, string vertices) {
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

/*bool check_input(string input) {
	if ( input.length() != 2 ) return false;
	return true;
}*/

// 1800 iterations
void scoreboard(edge *edges) {
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
	
	cout << endl << "Wyniki" << endl << endl;
	cout << "czerwony " << triangle_count[0] << " - " <<
		    triangle_count[1] << " niebieski" << endl << endl;
	
	if ( triangle_count[0] == 0 ) {
		cout << "Gracz czerwony wygrał" << endl;
	}
	else if ( triangle_count[1] == 0 ) {
		cout << "Gracz niebieski wygrał" << endl;
	}
}

int opp_color(int color) {
	if ( color == RED ) return BLUE;
	else return RED;
}

int empty_edges(edge *edges) {
	int count_edges = 0;
	for ( int i = 0 ; i < 15 ; i++ ) {
		if ( edges[i].color == BLACK ) count_edges++;
	}
	return count_edges;
}

void fill_last_edge(edge *edges, int color) {
	for ( int i = 0 ; i < 15 ; i++ ) {
		if ( edges[i].color == BLACK ) {
			edges[i].color = opp_color(color);
			break;
		}
	}
}

void reset_edges(edge *edges) {
	for ( int i = 0 ; i < 15 ; i++ ) {
		edges[i].color = BLACK;
	}
}

