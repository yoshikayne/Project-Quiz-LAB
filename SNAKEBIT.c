#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 40
#define HEIGHT 20
#define SIZE 6

// Declarations
// The array stores snake's coordinates
int snakeX[100], snakeY[100];

// Coordinates of snake's head and fruit
int x, y, fruitX, fruitY;

// The variable stores the length of the snake's tail
int snakeTailLen;

int gameover, key, score, choice, optMain, temp, playerIndex[SIZE];

// Pre-MainMenu function
void preMainMenu() {
    // The array store player's name
    printf("Please Enable Full-Screen Before Playing.\n\n");
    char playerName[100];
    do { 
        printf("Please Input Your Name: ");
        scanf("%s", playerName);
        system("cls");
    } while(sizeof(playerName) > 100);
}

// Leaderboard function
void leaderboard() {
    typedef struct{ //FORMAT PLAYER
    char name[100];
    int score;
    }player;
    player p[SIZE] = { //PENYIMPANAN DATA
        {" ", 0},
        {" ", 0},
        {" ", 0},
        {" ", 0},
        {" ", 0},
        {" ", 0} //YANG KE 6 INI BUAT DATA BARUNYA
    };
    process: //PENGULANGAN AJA
    printf("Name and Score: ");
    scanf("%s %d", p[playerIndex[5]].name, &p[playerIndex[5]].score); //INPUT NAMA DAN SKOR
    
    for(int i = 1; i <= SIZE; i++)  //simple initialization for the index array
        playerIndex[i] = i;

    //simple substitution sort, using the index
    for(int i = 1; i <= SIZE - 1; i++) {
        for(int j = i + 2; j <= SIZE; j++) {
            if(p[playerIndex[i]].score < p[playerIndex[j]].score) { //compare through the index
            temp = playerIndex[i];    //and swap only the indices
            playerIndex[i] = playerIndex[j];
            playerIndex[j] = temp;
            }
        } //BAGIAN INI DARI GOOGLE
    }

    printf("Players in unsorted order: \n\n");
    for(int i = 1; i <= SIZE; i++){
    printf("%20s  %d\n", p[i].name, p[i].score);
    }
    //HASIL DATA SORT TOP 5 (BISA DITAMBAH KALO MAU)
    printf("\nPlayers sorted by scores: \n\n");
    for(int i = 1; i <= 5; i++) {
    printf("%20s  %d\n", p[playerIndex[i]].name, p[playerIndex[i]].score);
    }
    
    goto process; //PENGULANGAN AJA
}


// Initial setup function
void setup() {
    gameover = 0;
    snakeTailLen = 0;

    // Initial coordinates of the snake
    x = WIDTH / 2;
    y = HEIGHT / 2;
    
    // Initial coordinates of the fruit
    fruitX = rand() % WIDTH;
    fruitY = rand() % HEIGHT;
    while(fruitX == 0)
        fruitX = rand() % WIDTH;

    while(fruitY == 0)
        fruitY = rand() % HEIGHT;

    // Initial score
    score = 0;
}

// Draw map function
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
            if(i == y && j == x){
                printf("O");
            }
            
            // Creating fruits with '*'
            else if(i == fruitY && j == fruitX) {
                printf("*");
            }
            
            // Creating snake's body with 'o'
            else {
                int prTail = 0;
                for(int k = 0 ; k < snakeTailLen; k++) {
                    if(snakeX[k] == j && snakeY[k] == i) {
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
    printf("SCORE = %d\n", score);
    printf("Press W, A, S, D for movement.\n");
    printf("Press X to quit the game.");
}

// Movement function
void input() {
    if(kbhit()) {
        switch (tolower(getch())) {
        case 'a':
            if(key!=2)
            key = 1;
            break;
        case 'd':
            if(key!=1)
            key = 2;
            break;
        case 'w':
            if(key!=4)
            key = 3;
            break;
        case 's':
            if(key!=3)
            key = 4;
            break;
        case 'x':
            gameover = 1;
            break;
        }
    }
}

// Function for the movement logic that checks eat, move and collisions
void logic() {
    // Updating the coordinates for continous movement of snake
    int prevX = snakeX[0];
    int prevY = snakeY[0];
    int prev2X, prev2Y;
    snakeX[0] = x;
    snakeY[0] = y;
    for (int i = 1; i < snakeTailLen; i++) {
        prev2X = snakeX[i];
        prev2Y = snakeY[i];
        snakeX[i] = prevX;
        snakeY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }
    
    // Changing the direction of movement of snake
    switch(key) {
        case 1:
            x--;
            break;
        case 2:
            x++;
            break;
        case 3:
            y--;
            break;
        case 4:
            y++;
            break;
        default:
            break;
    }

    // Game over scenarios
    if(x < 1 || x >= WIDTH + 2 || y < 1 || y >= HEIGHT + 1)
        gameover = 1;
 
    // Checks for collision with the tail (o)
    for(int i = 1; i <= snakeTailLen; i++) {
        if(snakeX[i] == x && snakeY[i] == y)
            gameover = 1;
    }

    // Score updates and generation of new fruit
    if(x == fruitX && y == fruitY) {
        fruitX = rand() % WIDTH;
        fruitY = rand() % HEIGHT;
        while(fruitX == 0) {
            fruitY = rand() % WIDTH;
        }

        while(fruitY == 0) {
            fruitY = rand() % HEIGHT;
            score += 10;
            snakeTailLen++;     
        }
    }
}

void game() {
    // Initial setup
    setup();

    // Game loop starts here

    while(!gameover) {
    map();
    input();
    logic();
    Sleep(70);
    }

    //Game Over
    if (gameover == 1) {
        do {
            system("cls");
            printf("=== GAME OVER ===\n");
            printf("SCORE: %d\n", score);
            printf("1. Restart Game\n");
            printf("2. Exit to Main Menu\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            switch(choice) {
                case 1:
                    printf("Restarting game...\n");
                    Sleep(3000);
                    game();
                break;

                case 2:
                    printf("Exiting to main menu...\n");
                    Sleep(3000);
                break;

                default:
                    printf("Invalid choice. Please choose 1 or 2.\n");
                    Sleep(400);
            }
        } while(choice < 1 || choice > 2);
    }
}

void mainMenu() {
    system("cls");
    do{
        printf(
        "\n"        
        "███████ ███    ██  █████  ██   ██ ███████ ██████  ██ ████████\n"
        "██      ████   ██ ██   ██ ██  ██  ██      ██   ██ ██    ██\n" 
        "███████ ██ ██  ██ ███████ █████   █████   ██████  ██    ██\n" 
        "     ██ ██  ██ ██ ██   ██ ██  ██  ██      ██   ██ ██    ██\n" 
        "███████ ██   ████ ██   ██ ██   ██ ███████ ██████  ██    ██\n" 
        "\n"
        "1. Play\n"
        "2. Leaderboard\n"
        "3. Exit\n"
        "\n"
        ">> "
        );

        scanf("%d", &optMain);

        switch(optMain) {
            case 1: //Play
                game();
            break;

            case 2: //Leaderboard
                leaderboard();
            break;

            case 3: //Exit
                printf("See you next time! :D\n");
                Sleep(3000);
            break;

            default: //Invalid input
                printf("Invalid option! Please input the correct option D:\n");
            break;
        }
    } while (optMain != 3);
}

int main() {
    // Pre-MainMenu
    preMainMenu();

    // Main Menu
    mainMenu();

    return 0;
}