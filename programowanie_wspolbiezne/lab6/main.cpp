#include <iostream>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <signal.h>
#include <stdlib.h>

#define ROW 3
#define COL 3
#define KEY 1234
#define CROSS 1
#define CIRCLE 2
#define DRAW 3
#define WIN 3

#define SEMAPHORE_X 0
#define SEMAPHORE_O 1

using namespace std;


int *gameboard = new int[ROW * COL];
int shmid, semid;
void *shared_memory;


int get_pos(int x, int y) {
	return 3*x + y;
}


void initialize_board() {
	
	
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			gameboard[get_pos(i, j)] = 0;
		}
	}
}



char get_value(int value) {
	switch(value) {
		case 0:
			return ' ';
		case 1:
			return 'X';
		case 2:
			return 'O';
		default:
			return '?';
	}
}


void show_board() {
	cout << " ___ ___ ___" << endl;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if ( j == 0 ) {
				cout << "| ";
			}
			cout << get_value(gameboard[get_pos(i, j)]) << " | ";
		}
		cout << endl;
		cout << " ___ ___ ___" << endl;
	}
}


int make_move(int pos_x, int pos_y, int sign) {
	if ( pos_x >= 0 && pos_x < ROW && pos_y >= 0 && pos_y < COL && gameboard[get_pos(pos_x, pos_y)] == 0 ) {
		gameboard[get_pos(pos_x, pos_y)] = sign;
		return 0;
	}
	cout << "Wrong position" << endl;
	return -1;
}


int game_status() {
	// horizontal and full check
	int all_count = 0;
	
	for (int i = 0; i < ROW; i++) {
		int col_count_x = 0, col_count_o = 0;
		for (int j = 0; j < COL; j++) {
			// full fields check
			if ( gameboard[get_pos(i, j)] == CROSS || gameboard[get_pos(i, j)] == CIRCLE )
				all_count++;
			
			// horizontal status
			if ( gameboard[get_pos(i, j)] == CROSS ) {
				col_count_x++;
			}
			else if ( gameboard[get_pos(i, j)] == CIRCLE ) {
				col_count_o++;
			}
		}
		if ( col_count_x == WIN )
			return CROSS;
		else if ( col_count_o == WIN )
			return CIRCLE;
	}
	
	if ( all_count == ROW * COL )
		return DRAW;
	
	
	
	// vertical status
	for (int i = 0; i < COL; i++) {
		int row_count_x = 0, row_count_o = 0;
		for (int j = 0; j < ROW; j++) {
			if ( gameboard[get_pos(j, i)] == CROSS ) {
				row_count_x++;
			}
			else if ( gameboard[get_pos(j, i)] == CIRCLE ) {
				row_count_o++;
			}
		}
		if ( row_count_x == WIN )
			return CROSS;
		else if ( row_count_o == WIN )
			return CIRCLE;
	}
	
	
	// diagonal status
	int pos1[3][2] = {{0, 0}, {1, 1}, {2, 2}};
	int pos2[3][2] = {{0, 2}, {1, 1}, {2, 0}};
	
	int diag1_count_x = 0, diag1_count_o = 0;
	int diag2_count_x = 0, diag2_count_o = 0; 
	
	for (int i = 0; i < 3; i++) {
		if ( gameboard[get_pos(pos1[i][0], pos1[i][1])] == CROSS )
			diag1_count_x++;
		else if ( gameboard[get_pos(pos1[i][0], pos1[i][1])] == CIRCLE )
			diag1_count_o++;
		
		if ( gameboard[get_pos(pos2[i][0], pos2[i][1])] == CROSS )
			diag2_count_x++;
		else if ( gameboard[get_pos(pos2[i][0], pos2[i][1])] == CIRCLE )
			diag2_count_o++;
	}
	
	if ( diag1_count_x == WIN || diag2_count_x == WIN )
		return CROSS;
	else if ( diag1_count_o == WIN || diag2_count_o == WIN )
		return CIRCLE;
	
	
	return 0;
}


void show_result() {
	cout << endl;
	
	if ( game_status() == CROSS )
		cout << "Cross wins!" << endl << endl;
	else if ( game_status() == CIRCLE )
		cout << "Circle wins!" << endl << endl;
	else
		cout << "Draw" << endl << endl;
}


void cleanup(int signal) {
	semctl(semid, 0, IPC_RMID, 0);
	shmctl(shmid, IPC_RMID, 0);
	exit(0);
}




int main(int argc, char *argv[]) {
	signal(SIGINT, cleanup);
	
	shmid = shmget(KEY, ROW * COL * sizeof(int), 0777|IPC_CREAT);
    shared_memory = shmat(shmid, 0, 0);
    gameboard = (int*)shared_memory;

    struct sembuf attemptMoveX={SEMAPHORE_X, -1, 0},
                  unlockO={SEMAPHORE_O, 1, 0},
                  attemptMoveO={SEMAPHORE_O, -1, 0},
                  unlockX={SEMAPHORE_X, 1, 0},
                  *attemptMove,
                  *unlockOpponent;
	
	
	int playerSign;
    if((semid = semget(KEY, 2, 0777|IPC_CREAT|IPC_EXCL)) > 0)
    {
		
        cout << "You are 1st player - X" << endl;
        playerSign = CROSS;
        semctl(semid, SEMAPHORE_X, SETVAL, 0);
        semctl(semid, SEMAPHORE_O, SETVAL, 0);
        attemptMove = &attemptMoveX;
        unlockOpponent = &unlockO;
    }
    else
    {
        cout << "You are 2st player - O" << endl;
        playerSign = CIRCLE;
        semid = semget(KEY, 2, 0777|IPC_CREAT);
        attemptMove = &attemptMoveO;
        unlockOpponent = &unlockX;
        semop(semid, unlockOpponent, 1);
    }
    
    
	initialize_board();
	bool running = true;
	while ( running ) {
		cout << "Waiting for opponent" << endl;
		semop(semid, attemptMove, 1);
		
		show_board();
		
		if ( game_status() > 0 ) {
			show_result();
			cleanup(0);
		}
		
		int pos_x, pos_y;
		do {
			cout << "Your move <row, col>: "; 
			cin >> pos_x >> pos_y;
		} while ( make_move(pos_x - 1, pos_y - 1, playerSign) == -1 );
		
		show_board();
		semop(semid, unlockOpponent, 1);
	}
	
	//show_result();
	cleanup(0);
	
	return 0;
}
