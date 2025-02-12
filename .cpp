#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <climits>  // For INT_MAX
#ifdef _WIN32  // Windows-specific headers
#include <conio.h>
#include <windows.h>
#define CLEAR_SCREEN "cls"
#else  // macOS and Linux headers
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#define CLEAR_SCREEN "clear"
#endif

using namespace std;

const int WIDTH = 60;
const int HEIGHT = 30;
enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

#ifdef _WIN32
void sleep_ms(int milliseconds) { Sleep(milliseconds); }
#else
void sleep_ms(int milliseconds) { usleep(milliseconds * 1000); }
#endif

#ifdef _WIN32
int kbhit() { return _kbhit(); }
int getch() { return _getch(); }
#else
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    
    ch = getchar();
    
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

class SnakeGame {
private:
    bool gameOver;
    int score, speed;
    int x, y, foodX, foodY, tailX[100], tailY[100], tailLength;
    Direction dir;
    static int highScore;
    static int lowScore;
    char headChar;
    char bodyChar;

    void spawnFood() {
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
    }

public:
    SnakeGame(int difficulty) : headChar('@'), bodyChar('o') {
        switch (difficulty) {
            case 1: speed = 150; break; // Easy
            case 2: speed = 100; break; // Medium
            case 3: speed = 50; break;  // Hard
            default: speed = 100;
        }
        resetGame();
    }

    void resetGame() {
        if (score > highScore) highScore = score;
        if (score < lowScore && score > 0) lowScore = score;
        
        gameOver = false;
        dir = STOP;
        x = WIDTH / 2;
        y = HEIGHT / 2;
        score = 0;
        tailLength = 3;
        spawnFood();
    }

    void draw() {
        system(CLEAR_SCREEN);
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << "#";
                if (i == y && j == x) cout << headChar;
                else if (i == foodY && j == foodX) cout << "F";
                else {
                    bool printTail = false;
                    for (int k = 0; k < tailLength; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << bodyChar;
                            printTail = true;
                        }
                    }
                    if (!printTail) cout << " ";
                }
                if (j == WIDTH - 1) cout << "#";
            }
            cout << endl;
        }
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << "\nScore: " << score << " | High Score: " << highScore << " | Low Score: " << (lowScore == INT_MAX ? 0 : lowScore);
        cout << "\nControls: W/A/S/D to move, X to quit, R to retry" << endl;
    }

    void input() {
        if (kbhit()) {
            switch (getch()) {
                case 'w': if (dir != DOWN) dir = UP; break;
                case 's': if (dir != UP) dir = DOWN; break;
                case 'a': if (dir != RIGHT) dir = LEFT; break;
                case 'd': if (dir != LEFT) dir = RIGHT; break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {
        int prevX = tailX[0], prevY = tailY[0], prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < tailLength; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }
        switch (dir) {
            case UP: y--; break;
            case DOWN: y++; break;
            case LEFT: x--; break;
            case RIGHT: x++; break;
            default: break;
        }
        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) gameOver = true;
        for (int i = 4; i < tailLength; i++)
            if (tailX[i] == x && tailY[i] == y) gameOver = true;
        if (x == foodX && y == foodY) {
            score += 10;
            tailLength++;
            spawnFood();
        }
    }

    bool isGameOver() { return gameOver; }

    int getSpeed() { return speed; }
};

int SnakeGame::highScore = 0;
int SnakeGame::lowScore = INT_MAX;

int main() {
    srand(time(0));
    while (true) {
        int difficulty;
        cout << "Select Difficulty (1: Easy, 2: Medium, 3: Hard): ";
        cin >> difficulty;
        SnakeGame game(difficulty);
        while (!game.isGameOver()) {
            game.draw();
            game.input();
            game.logic();
            sleep_ms(game.getSpeed());
        }
        cout << "\nGame Over! Press R to retry or X to exit.\n";
        char choice;
        cin >> choice;
        if (choice == 'x' || choice == 'X') break;
    }
    cout << "\nThanks for playing!\n";
    return 0;
}
