#include <iostream>
#include <cctype>

#define USED -1
#define INVALID -2

#define BLACK 0
#define RED 1
#define BLUE 2

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
	bool running = true;
	char *vertices = new char[2];
	int color = RED;
	int position;
	int remained_edges = 15;
	
	initial_edges();
	
	
	while ( running ) {
		cout << "Pozostało wierzchłków: " << remained_edges << endl;
		
		
		while ( true ) {
			cout << "Podaj dwa wierzchołki: ";
			cin >> vertices;
			if ( !check_input(vertices) ) {
				cout << "Za duża/mała liczba wierzochłków" << endl;
				continue;
			}
			
			vertices[0] = toupper(vertices[0]);
			vertices[1] = toupper(vertices[1]);
			
			position = make_move(vertices);
			if ( position >= 0 ) {
				edges[position].color = color;
				remained_edges--;
				break;
			}
			else if ( position == USED ) cout << "Ta krawędź jest już zajęta. Proszę wybrać inną" << endl;
			else if ( position == INVALID ) cout << "Podano nieistniejącą krawędź. Spróbuj ponownie" << endl;
		}
		
		show_edges();
		
		
		
		// socket there
		if ( color == RED ) color = BLUE;
		else color = RED;
		
		if ( remained_edges <= 0 ) {
			scoreboard();
			running = false;
		}
	}
	
	return 0;
}
