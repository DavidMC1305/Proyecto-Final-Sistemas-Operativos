#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define ROWS 10
#define COLS 10
#define SHIPS 5
#define SHIP_SIZE 3

sem_t mutex;
int player1_board[ROWS][COLS];
int player2_board[ROWS][COLS];
int player1_hits[ROWS][COLS];
int player2_hits[ROWS][COLS];

bool player1_turn = true;
bool game_over = false;

pthread_t player1_thread;
pthread_t player2_thread;

// Función para inicializar los tableros
void initialize_board(int board[ROWS][COLS]) {
    int i, j;
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            board[i][j] = 0;
        }
    }
}

// Función para imprimir el tablero
void print_board(int board[ROWS][COLS]) {
    int i, j;
    printf("  ");
    for (i = 0; i < COLS; i++) {
        printf("%d ", i);
    }
    printf("\n");
    for (i = 0; i < ROWS; i++) {
        printf("%d ", i);
        for (j = 0; j < COLS; j++) {
            printf("%d ", board[i][j]);
        }
        printf("\n");
    }
}

// Función para colocar los barcos
void place_ships(int board[ROWS][COLS]) {
    int i, j, k;
    srand(time(NULL));
    for (k = 0; k < SHIPS; k++) {
        bool valid = false;
        int x, y, dx, dy;
        while (!valid) {
            x = rand() % ROWS;
            y = rand() % COLS;
            dx = rand() % 3 - 1;
            dy = rand() % 3 - 1;
            valid = true;
            for (i = 0; i < SHIP_SIZE; i++) {
                int newx = x + i*dx;
                int newy = y + i*dy;
                if (newx < 0 || newx >= ROWS || newy < 0 || newy >= COLS || board[newx][newy] != 0) {
                    valid = false;
                    break;
                }
            }
        }
        for (i = 0; i < SHIP_SIZE; i++) {
            board[x+i*dx][y+i*dy] = k+1;
        }
    }
}

// Función para realizar un ataque
void attack(int x, int y, int board[ROWS][COLS], int hits[ROWS][COLS]) {
    if (board[x][y] == 0) {
        hits[x][y] = 1;
    } else {
        hits[x][y] = 2;
        int i, j;
        bool sunk = true;
        for (i = 0; i < ROWS; i++) {
            for (j = 0; j < COLS; j++) {
                if (board[i][j] == board[x][y] && hits[i][j] == 0) {
                    sunk = false;

 }
        }
    }
    if (sunk) {
        printf("You have sunk an enemy ship!\n");
    } else {
        printf("Ship touched!\n");
    }
}



}

// Función para manejar las señales

 void signal_handler(int signo) {
	if (signo == SIGALRM) {
		printf("The time is up!\n");
 	game_over = true;

 }

}

// Función para el jugador 1

void* player1(void* arg) {
	while (!game_over) {
		if (player1_turn) {
	int x, y;

	printf("Player 1 board:\n");
	print_board(player1_board);
	printf("Player 1 attack board:\n");
	print_board(player1_hits);

	printf("Player 1, enter the position to attack (x y): ");


	scanf("%d %d", &x, &y);
        sem_wait(&mutex); // Esperar a que el semáforo esté disponible
	attack(x, y, player2_board, player1_hits);
        sem_post(&mutex); // Liberar el semáforo después del ataque
	player1_turn = false;

	}

    }

 return NULL;

 }


// Función para el jugador 2

void* player2(void* arg) {

	while (!game_over) {

	if (!player1_turn) {

	int x, y;

	printf("PLayer 2 board:\n");
	print_board(player2_board);
	printf("Player 2 attack board:\n");
	print_board(player2_hits);

	printf("Player 2, enter the position to attack (x y): ");

	scanf("%d %d", &x, &y);
        sem_wait(&mutex); // Esperar a que el semáforo esté disponible
	attack(x, y, player1_board, player2_hits);
        sem_post(&mutex); // Liberar el semáforo después del ataque

	player1_turn = true; } 

	} 
   return NULL; 

}


int main() {

	printf("Final Project: Naval Battle Game \n");
	printf("David Mayorga Castelan \n");
	printf("The grid is made up of a total of 100 cells. \n");

sem_init(&mutex, 0, 1);

initialize_board(player1_board);

initialize_board(player2_board);

place_ships(player1_board);

place_ships(player2_board);

signal(SIGALRM, signal_handler);
alarm(60);

pthread_create(&player1_thread, NULL, player1, NULL);
pthread_create(&player2_thread, NULL, player2, NULL);

pthread_join(player1_thread, NULL);
pthread_join(player2_thread, NULL);

printf("Game over!\n");
printf("Player 1 board:\n");
print_board(player1_board);
printf("Player 1 attack board:\n");
print_board(player1_hits);
printf("Player 2 board:\n");
print_board(player2_board);
printf("Player 2 attack board:\n");
print_board(player2_hits);


return 0;
}
