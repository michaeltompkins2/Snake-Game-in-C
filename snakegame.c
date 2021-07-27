#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define GRASS ' '
#define GRASS_PAIR 1
#define BORDERSNAKEPAIR 2
#define StarPair 3
#define TROPHY1 '1'
#define TROPHY2 '2'
#define TROPHY3 '3'
#define TROPHY4 '4'
#define TROPHY5 '5'
#define TROPHY6 '6'
#define TROPHY7 '7'
#define TROPHY8 '8'
#define TROPHY9 '9'


// Michael Tompkins, Dustin Chasatheny, Keondre Lucas, Antoan
// To Run the Program:
// gcc -lcurses snakegame.c -o test
// ./test

// Snake Pit Dimensions
#define PIT_WIDTH 50  // defining the width of the pit
#define PIT_HEIGHT 30 // defining the height of the pit
#define TOPBOTBORDER                                                           \
  '-' // defining the character for the top and bottom of the pit walls
#define LEFTRIGHTBORDER                                                        \
  '|'               // defining the character for the left and right pit walls
int score = 0;      // initializing a global variable for the score
int speed = 250000; // initializing a global variable for the speed
int counttime =
    200; // initializing the int variable to count to before moving the trophy
int looping = 0; // the counting variable that moves the trophy

int snakeLength = 3; // Starting snake length

// struct pointer to dimensions of the screen
struct point {
  int x;
  int y;
} snake[80]; // Snake in an array, max snake length 80

// struct pointer
typedef struct point position;
position CurrentDirection; // user's old position 
position UpdateDirection; //user's new direction
int gameover = 0; 
int chara;

position snakeTrophy;
// moving in the given direction 
const position UserUp = {0, -1};
const position UserDown = {0, 1};
const position UserLeft = {-1, 0};
const position UserRight = {1, 0};

char CreateRandomNum();            // create a random ascii number
void StartingPosition(void);       // where the snake will start
void snakeDirection(void);         // moving the entire snake
int CollisionCheck(void);          // check if snake crashed
void CreatePit(void);              // create pit
void CreateSnake(void);            // make the snake
void NewKeyDirection(void);        // user input to chang direction
int oppDirect(position, position); // if the snake moves on to itself
int createrandomint();   // random timer for trophy to be on the screen
void randomTrophy(void); // create a random trophy at a random spot
int snakeEat(
    int, int); // when the snake is at the same x and y as the trophy x and y

int main() {
  initscr(); // Initializing the curses library
  noecho(); // Preventing the terminal from printing out user's key press on the
            // screen.

  cbreak();
  keypad(stdscr, TRUE);  // Enabling the user input from the keyboard
  nodelay(stdscr, TRUE); // no delay on the screen!
  curs_set(0);           // Make the cursor invisible
  CreatePit();

  StartingPosition();
  // while the game is not over it will call these functions to continue the game
  while (!gameover) { 
    CreateSnake();
    NewKeyDirection(); // user keystokes everytime it loops
    usleep(speed); // speed of the game
    curs_set(0);
    createrandomint();
    looping = looping + createrandomint(); 
  }
  clear();
  refresh();
  endwin();
  return 0;
}

// Function to create the snake pit

void CreatePit() {

  clear(); 

  mvprintw(
      32, 95,
      "S _ N _ A _ K _ E "); // printing the game title to the given position
  mvprintw(
      34, 80,
      "      Press x to quit the game . . ."); // printing an option to quit the
                                               // game to the screen

  mvprintw(8, 135, "       ---_ ......._-_--."); // printing the snake graphic
                                                 // to the right of the pit
  mvprintw(9, 135, "      (|\\ /      / /| \\  \\ ");
  mvprintw(10, 135, "      /  /     .'  -=-'   `.");
  mvprintw(11, 135, "     /  /    .'             )");
  mvprintw(12, 135, "   _/  /   .'        _.)   / ");
  mvprintw(13, 135, "  / o   o        _.-' /  .'");
  mvprintw(14, 135, "  \\          _.-'    / .'*|");
  mvprintw(15, 135, "   \\______.-'//    .'.' \\*|");
  mvprintw(16, 135, "    \\|  \\ | //   .'.' _ |*|");
  mvprintw(17, 135, "     `   \\|//  .'.'_ _ _|*|");
  mvprintw(18, 135, "      .  .// .'.' | _ _ \\*|");
  mvprintw(19, 135, "      /^|            \\ _ _ \\*");
  mvprintw(20, 135, "     '  `             \\ _ _ \\      ASH (+VK)");
  mvprintw(21, 135, "                      \\_");

  mvprintw(8, 45, "       ---_ ......._-_--."); // printing the snake graphic to
                                                // the left of the pit.
  mvprintw(9, 45, "      (|\\ /      / /| \\  \\ ");
  mvprintw(10, 45, "      /  /     .'  -=-'   `.");
  mvprintw(11, 45, "     /  /    .'             )");
  mvprintw(12, 45, "   _/  /   .'        _.)   / ");
  mvprintw(13, 45, "  / o   o        _.-' /  .'");
  mvprintw(14, 45, "  \\          _.-'    / .'*|");
  mvprintw(15, 45, "   \\______.-'//    .'.' \\*|");
  mvprintw(16, 45, "    \\|  \\ | //   .'.' _ |*|");
  mvprintw(17, 45, "     `   \\|//  .'.'_ _ _|*|");
  mvprintw(18, 45, "      .  .// .'.' | _ _ \\*|");
  mvprintw(19, 45, "      /^|            \\ _ _ \\*");
  mvprintw(20, 45, "     '  `             \\ _ _ \\      ");
  mvprintw(21, 45, "                      \\_");


  int i = 0;
  int j = 1;
  while (i < PIT_WIDTH) {  // while we have not reached the end of the pit width
    attron(COLOR_PAIR(1)); // turn on the color pair, color not working
    mvaddch(0, i + 81, TOPBOTBORDER); // print the topbotbotder character in
                                      // each x,y position to create top wall
    mvaddch(PIT_HEIGHT + 1, i + 80,
            TOPBOTBORDER);  // print the topbotbotder character in each x,y
                            // position to create bottom wall
    attroff(COLOR_PAIR(1)); // turn off color pair, not working
    i++;                    // increment the position
  }
  mvaddch(0, 80, '*'); // add the star characters to each edge of the pit
  mvaddch(0, 130, '*');
  mvaddch(PIT_HEIGHT + 1, 130, '*');
  mvaddch(PIT_HEIGHT + 1, 80, '*');

  // while we have not reached the end of the pit height
  while (j <= PIT_HEIGHT) {
    attron(COLOR_PAIR(1));
    mvaddch(j, 80, LEFTRIGHTBORDER);             // left of wall
    mvaddch(j, PIT_WIDTH + 80, LEFTRIGHTBORDER); // right of wall
    attroff(COLOR_PAIR(1));
    j++;
  }
  // attroff(COLOR_PAIR(BORDERSNAKEPAIR));
}

// Creating the snake
void CreateSnake() {
  for (int i = 0; i < snakeLength; i++) {
    mvaddch(snake[i].y + 0, snake[i].x + 80, 'o');
    // win condition so snake has to greater or equal to the perimeter
    if (snakeLength >= ((PIT_WIDTH * 2) + (PIT_HEIGHT * 2)) / 2) {
      clear();
      printf("You win!\n");
      usleep(15000);
      exit(0);
    }
  }
}
void createstrophy() {
  char arr[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};
  int randy = rand() % 9;
  mvaddch(snakeTrophy.y + 1, snakeTrophy.x + 80, CreateRandomNum());
}
int createrandomint() { // creating a random int from 1-9
  int randy = rand() % 9;
  return randy;
}
char CreateRandomNum() { // creating a random ascii from 1-9
  int kl = (rand() % 9) + 1;
  char number2 = (char)kl;

  // converting to ASCII
  if (kl >= 10) {
    number2 = 'A' + number2 - 10;
  } else {
    number2 = '0' + number2;
  };

  return number2;
}
// user's input
void NewKeyDirection() {

  if ((chara = getch()) != ERR) {
    switch (chara) {
    case KEY_UP:
      UpdateDirection = UserUp;

      break;
    case KEY_DOWN:
      UpdateDirection = UserDown;
      break;
    case KEY_LEFT:
      UpdateDirection = UserLeft;
      break;
    case KEY_RIGHT:
      UpdateDirection = UserRight;
      break;
    case 'x': // Press x to exit program.
      gameover = 1;
      break;
    default:
      break;
    }
    if (!oppDirect(CurrentDirection, UpdateDirection)) {
      CurrentDirection = UpdateDirection;
    }
    if (oppDirect(CurrentDirection, UpdateDirection)) {
      endwin();
      exit(1);
    }
  }
  curs_set(0);
  CreatePit();
  CreateSnake();
  createstrophy();
  snakeDirection();

  // if the snake has eaten the trophy the snake will increase speed
  if (snake[0].x == snakeTrophy.x && snake[0].y == snakeTrophy.y) {
    int testch;
    speed = speed - 20000;

    snakeLength =
        snakeLength +
        createrandomint(); // add the number to the snake which ever it lands on
                           /*
                               testch = mvinch(snakeTrophy[0].x, snakeTrophy[0].y);
                               if ((testch & A_CHARTEXT) == TROPHY1){
                                   snakeLength = snakeLength + 1;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY2){
                                   snakeLength = snakeLength + 2;
                               }
                               if ((testch & A_CHARTEXT)  == TROPHY3){
                                   snakeLength = snakeLength + 3;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY4){
                                   snakeLength = snakeLength + 4;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY5){
                                   snakeLength = snakeLength + 5;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY6){
                                   snakeLength = snakeLength + 6;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY7){
                                   snakeLength = snakeLength + 7;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY8){
                                   snakeLength = snakeLength + 8;
                               }
                               if ((testch & A_CHARTEXT) == TROPHY9){
                                   snakeLength = snakeLength + 9;
                               } */

    randomTrophy(); // creates new trophy to be displayed
  }
  if (CollisionCheck()) {
    gameover = 1;
  }
  if (looping >= 100) { // expire if trophy is not eaten will spawn in another one
    randomTrophy();
    looping = 0;
  }
}

// check if going opposite direction
int oppDirect(position a, position b) {
  if (a.x == -b.x && a.y == -b.y) {
    return 1;
  }
  return 0;
}

// start in the middle but different direction every time
void StartingPosition() {
  int randomdirection = rand() % 3;
  switch (randomdirection) {
  case (0):
    CurrentDirection = UserRight;
  case (1):
    CurrentDirection = UserLeft;
  case (2):
    CurrentDirection = UserUp;
  case (3):
    CurrentDirection = UserDown;
  }
  for (int i = 0; i < snakeLength; i++) {
    snake[i].x = PIT_WIDTH / 2 - i;
    snake[i].y = PIT_HEIGHT / 2;
  }
  // Add in a trophy
  randomTrophy();
}

// Moving the entire snake
void snakeDirection() {
  for (int i = snakeLength; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  snake[0].x += CurrentDirection.x;
  snake[0].y += CurrentDirection.y;
}

// checking for collisions, if the snake collides with the wall or itself
int CollisionCheck() {
  if (snake[0].x >= PIT_WIDTH || snake[0].x <= 0 ||
      snake[0].y >= PIT_HEIGHT + 1 || snake[0].y <= 0) {
    return 1;
  }
  for (int i = 1; i < snakeLength; i++) {
    if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
      return 1;
    }
  }
  return 0;


// Moves Trophy to new spot when snake eats it
void randomTrophy() {
  do {
      char arr[] = {'1','2','3','4','5', '6', '7', '8', '9'};
    int randy = rand() % 20;
mvaddch(snakeTrophy.y + 1, snakeTrophy.x + 80, arr[randy]);
    snakeTrophy.x = rand() % (PIT_WIDTH - 2) + 1;
    snakeTrophy.y = rand() % (PIT_HEIGHT - 2) + 1;


  }while (snakeEat(snakeTrophy.x, snakeTrophy.y) || snakeTrophy.x < 0 ||
           snakeTrophy.x >= PIT_WIDTH || snakeTrophy.y < 0 ||
           snakeTrophy.y >= PIT_HEIGHT);
}

// check if snake is that the position of the trophy
int snakeEat(int x, int y) {
  for (int i = 0; i < snakeLength; i++) {
    if (snake[i].x == x && snake[i].y == y) {
      return 1;
    }
  }
  return 0;
}
