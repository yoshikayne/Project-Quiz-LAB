#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>

#define WIDTH 40
#define HEIGHT 20
#define MAX_NAME_LENGTH 100
#define MAX_PLAYERS 100

// Structure to store player data
typedef struct {
    char name[MAX_NAME_LENGTH];
    int score;
} Player;

// Structure to store game state
typedef struct {
    int snakeX[MAX_PLAYERS], snakeY[MAX_PLAYERS];
    int x, y, fruitX, fruitY;
    int snakeTailLen;
    int gameover, key, score, choice, optMain, temp;
    Player currentPlayer;
} GameState;

GameState gameState;

// Function prototypes
void preMainMenu();
void setup();
void map();
void input();
void logic();
void game();
int comparePlayers(const void *a, const void *b);
void loadLeaderboard(Player players[], int *size);
void saveLeaderboard(Player players[], int size);
void displayLeaderboard(Player players[], int size);
void updateLeaderboard(char playerName[], int playerScore);
void leaderboard();
void searchPlayer();
void mainMenu();

// Function
// Pre-MainMenu function
void preMainMenu() {
    printf("Please Enable Full-Screen Before Playing For Maximum Experience.\n\n");
    Sleep(3000);
    printf("Loading Game");
    Sleep(700);
    printf(".");
    Sleep(700);
    printf(".");
    Sleep(700);
    printf(".\n");
    Sleep(700);
    system("cls");
    printf("Welcome to SnakeBit!\n");
    Sleep(700);
    
    do { 
        printf("Please Input Your Name (max 100): ");
        scanf("%s", gameState.currentPlayer.name);
        system("cls");
    } while(strlen(gameState.currentPlayer.name) > MAX_NAME_LENGTH - 1);
}

// Setup function
void setup() {
    gameState.gameover = 0;
    gameState.snakeTailLen = 0;

    // Initial coordinates of the snake
    gameState.x = WIDTH / 2;
    gameState.y = HEIGHT / 2;
    
    // Initial coordinates of the fruit
    gameState.fruitX = rand() % WIDTH;
    gameState.fruitY = rand() % HEIGHT;
    while(gameState.fruitX == 0)
        gameState.fruitX = rand() % WIDTH;

    while(gameState.fruitY == 0)
        gameState.fruitY = rand() % HEIGHT;

    // Initial score
    gameState.currentPlayer.score = 0;
}

// Map function
void map() {
    // Clear screen
    system("cls");
    
    // Creating top wall with '-'
    for(int i = 1; i <= WIDTH + 2; i++) {
        printf("-");
    }
    printf("\n");
    
    for(int i = 1; i <= HEIGHT; i++) {
        for(int j = 1; j <= WIDTH + 1; j++) {

            // Creating side walls with '|'
            if(j == 1 || j == WIDTH + 1) 
                printf("|");
            
            // Creating snake's head with 'O'
            if(i == gameState.y && j == gameState.x){
                printf("O");
            }
            
            // Creating fruits with '*'
            else if(i == gameState.fruitY && j == gameState.fruitX) {
                printf("*");
            }
            
            // Creating snake's body with 'o'
            else {
                int prTail = 0;
                for(int k = 0 ; k < gameState.snakeTailLen; k++) {
                    if(gameState.snakeX[k] == j && gameState.snakeY[k] == i) {
                        printf("o");
                        prTail = 1;
                    }
                }
                if(!prTail)
                    printf(" ");
            }
        }
      printf("\n");
    }
    
    // Creating bottom wall with '-'
    for(int i = 1; i <= WIDTH + 2; i++){
        printf("-");
    }
    printf("\n");

    // Print score and instructions
    printf("SCORE = %d\n", gameState.currentPlayer.score);
    printf("Press W, A, S, D for movement.\n");
    printf("Press X to exit the game.");
}

// Keyboard input function
void input() {
    if(kbhit()) {
        switch (tolower(getch())) {
        case 'a':
            if(gameState.key != 2)
                gameState.key = 1;
            break;

        case 'd':
            if(gameState.key != 1)
                gameState.key = 2;
            break;

        case 'w':
            if(gameState.key != 4)
                gameState.key = 3;
            break;

        case 's':
            if(gameState.key != 3)
                gameState.key = 4;
            break;

        case 'x':
            printf("Exiting game");
            Sleep(700);
            printf(".");
            Sleep(700);
            printf(".");
            Sleep(700);
            printf(".");
            gameState.gameover = 1;
            break;
        }
    }
}


// Logic function
void logic() {
    // Updating the coordinates for continuous movement of snake
    int prevX = gameState.snakeX[0];
    int prevY = gameState.snakeY[0];
    int prev2X, prev2Y;
    gameState.snakeX[0] = gameState.x;
    gameState.snakeY[0] = gameState.y;

    for (int i = 1; i < gameState.snakeTailLen; i++) {
        prev2X = gameState.snakeX[i];
        prev2Y = gameState.snakeY[i];
        gameState.snakeX[i] = prevX;
        gameState.snakeY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    // Changing the direction of movement of snake
    switch(gameState.key) {
        case 1:
            gameState.x--;
            break;
        case 2:
            gameState.x++;
            break;
        case 3:
            gameState.y--;
            break;
        case 4:
            gameState.y++;
            break;
        default:
            break;
    }

    // Game over scenarios
    if(gameState.x < 1 || gameState.x >= WIDTH + 2 || gameState.y < 1 || gameState.y >= HEIGHT + 1)
        gameState.gameover = 1;
 
    // Checks for collision with the tail (o)
    for(int i = 1; i <= gameState.snakeTailLen; i++) {
        if(gameState.snakeX[i] == gameState.x && gameState.snakeY[i] == gameState.y)
            gameState.gameover = 1;
    }

    // Score updates and generation of new fruit
    if(gameState.x == gameState.fruitX && gameState.y == gameState.fruitY) {
        gameState.fruitX = rand() % WIDTH;
        gameState.fruitY = rand() % HEIGHT;
        while(gameState.fruitX == 0) {
            gameState.fruitX = rand() % WIDTH;
        }
        while(gameState.fruitY == 0) {
            gameState.fruitY = rand() % HEIGHT;
        }
        gameState.currentPlayer.score += 10;
        gameState.snakeTailLen++;
    }
}

// Game function
void game() {
    // Game loop
    while (1) {
        setup();
        while (!gameState.gameover) {
            map();
            input();
            logic();
            Sleep(70);
        }

        // Game Over
        do {
            system("cls");
            printf("=== GAME OVER ===\n");
            printf("SCORE: %d\n", gameState.currentPlayer.score);
            updateLeaderboard(gameState.currentPlayer.name, gameState.currentPlayer.score);
            printf("1. Restart Game\n");
            printf("2. Exit to Main Menu\n");
            printf(">> ");
            scanf("%d", &gameState.choice);
            switch (gameState.choice) {
                case 1:
                    break;
                case 2:
                    printf("Returning to main menu");
                    Sleep(700);
                    printf(".");
                    Sleep(700);
                    printf(".");
                    Sleep(700);
                    printf(".");
                    system("cls");
                    return;
                default:
                    printf("Invalid choice. Please choose 1 or 2.\n");
                    Sleep(500);
            }
        } while (gameState.choice < 1 || gameState.choice > 2);
    }
}

// Compare function for qsort
int comparePlayers(const void *a, const void *b) {
    Player *playerA = (Player *)a;
    Player *playerB = (Player *)b;
        return playerB->score - playerA->score;
    }

// Load leaderboard from file
void loadLeaderboard(Player players[], int *size) {
    FILE *lb = fopen("leaderboard.txt", "r");
    if (lb == NULL) {
        *size = 0;
        return;
    }
    *size = 0;
    while (fscanf(lb, "%s - %d", players[*size].name, &players[*size].score) != EOF) {
        (*size)++;
    }
    fclose(lb);
}

// Save leaderboard to file
void saveLeaderboard(Player players[], int size) {
    FILE *lb = fopen("leaderboard.txt", "w");
    if (lb == NULL) {
        perror("Error opening file");
        return;
    }
    for (int i = 0; i < size; i++) {
        fprintf(lb, "%s - %d\n", players[i].name, players[i].score);
    }
    fclose(lb);
}


// Update leaderboard with new player score
void updateLeaderboard(char playerName[], int playerScore) {
    Player players[MAX_PLAYERS];
    int size;
    loadLeaderboard(players, &size);

    // Check if player already exists in leaderboard
    int found = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(players[i].name, playerName) == 0) {
            if (playerScore > players[i].score) {
                players[i].score = playerScore;
            }
            found = 1;
            break;
        }
    }

    // If player doesn't exist, add new entry
    if (!found && size < MAX_PLAYERS) {
        strncpy(players[size].name, playerName, MAX_NAME_LENGTH - 1);
        players[size].name[MAX_NAME_LENGTH - 1] = '\0';
        players[size].score = playerScore;
        size++;
    }

    // Sort leaderboard
    qsort(players, size, sizeof(Player), comparePlayers);

    // Save updated leaderboard
    saveLeaderboard(players, size);
}

void displayLeaderboard(Player players[], int size) {
    printf("=== LEADERBOARD ===\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s - %d\n", i + 1, players[i].name, players[i].score);
    }
}

// Leaderboard function
void leaderboard() {
    system("cls");
    Player players[MAX_PLAYERS];
    int size;
    loadLeaderboard(players, &size);
    displayLeaderboard(players, size);
    printf("\n");
    printf("1. Search Player\n");
    printf("2. Return to Main Menu\n");
    printf(">> ");
    scanf("%d", &gameState.choice);
    switch (gameState.choice) {
        case 1:
            searchPlayer();
            break;
        case 2:
            printf("Returning to main menu");
            Sleep(700);
            printf(".");
            Sleep(700);
            printf(".");
            Sleep(700);
            printf(".");
            system("cls");
            return;
            
        default:
            printf("Invalid choice. Please choose 1 or 2.\n");
            Sleep(500);
    }
}

// Search player in leaderboard
void searchPlayer() {
    system("cls");
    Player players[MAX_PLAYERS];
    int size;
    loadLeaderboard(players, &size);

    char searchName[MAX_NAME_LENGTH]; printf("Enter player name to search: "); scanf("%s", searchName);

    int found = 0;
    for (int i = 0; i < size; i++) {
        if (strcmp(players[i].name, searchName) == 0) {
            printf("Player found: %s - %d\n", players[i].name, players[i].score);
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Player not found.\n");
    }

    printf("\nPress any key to return to the main menu...\n"); getch();
    printf("Returning to main menu");
    Sleep(700);
    printf(".");
    Sleep(700);
    printf(".");
    Sleep(700);
    printf(".");
    system("cls");
    return;
}

// MainMenu function
void mainMenu() {
    system("cls");
    do{
        printf(
            "\n"
            "███████╗███╗   ██╗ █████╗ ██╗  ██╗███████╗██████╗ ██╗████████╗\n"
            "██╔════╝████╗  ██║██╔══██╗██║ ██╔╝██╔════╝██╔══██╗██║╚══██╔══╝\n"
            "███████╗██╔██╗ ██║███████║█████╔╝ █████╗  ██████╔╝██║   ██║   \n"
            "╚════██║██║╚██╗██║██╔══██║██╔═██╗ ██╔══╝  ██╔══██╗██║   ██║   \n"
            "███████║██║ ╚████║██║  ██║██║  ██╗███████╗██████╔╝██║   ██║   \n"
            "╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚═════╝ ╚═╝   ╚═╝   \n"
            "\n"
            "1. Play\n"
            "2. Leaderboard\n"
            "3. Exit\n"
            "\n"
            ">> "
        );

        scanf("%d", &gameState.optMain);

        switch(gameState.optMain) {
            case 1: //Play
                game();
            break;

            case 2: //Leaderboard
                leaderboard();
            break;

            case 3: //Exit
                printf("See you next time! ");
                Sleep(700);
                printf(":");
                Sleep(700);
                printf("D");
                Sleep(700);
                printf("!");
                Sleep(1000);
            break;

            default: //Invalid input
                printf("Invalid option! Please input the correct option D:\n");
            break;
        }
    } while (gameState.optMain != 3);
}

int main() {
    // Pre-MainMenu
    preMainMenu();

    // Main Menu
    mainMenu();

    return 0;
}