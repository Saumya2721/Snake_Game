#include <iostream>
#include <cstdlib>  // For rand() and srand()
#include <ctime>    // For time()
#include <unistd.h> // For usleep()
#ifdef _WIN32
    #include <conio.h> // For _getch() and kbhit() (Windows only)
    #define CLEAR_SCREEN "cls"
#else
    #include <termios.h>
    #include <fcntl.h>
    #define CLEAR_SCREEN "clear"
    
    // Function to get a single character without Enter key (Linux/macOS)
    int _getch() {
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

    // Function to check if a key is pressed (Linux/macOS)
    int _kbhit() {
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
#endif

using namespace std;

// Function to create a delay (uses usleep on Unix systems)
void timedelay(int milliseconds) {
        clock_t start_time = clock();
        while (clock() < start_time + milliseconds * CLOCKS_PER_SEC / 1000);
}
   

// Constants for game dimensions
const int WIDTH = 60;
const int HEIGHT = 30;

// Enumeration for directions
enum Direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

// SnakeGame Class
class SnakeGame {
private:
    bool gameOver;
    int score;
    static int highScore;
    int x, y;  // Snake's head position
    int foodX, foodY;  // Food position
    int tailX[100], tailY[100];  // Snake's tail positions
    int tailLength;  // Length of the snake's tail
    Direction dir;  // Current direction of the snake

    void spawnFood() {
        foodX = rand() % WIDTH;
        foodY = rand() % HEIGHT;
        for (int i = 0; i < tailLength; i++) {
            if (foodX == tailX[i] && foodY == tailY[i]) {
                spawnFood();
                break;
            }
        }
    }

public:
    SnakeGame() { resetGame(); }

    void resetGame() {
        gameOver = false;
        dir = STOP;
        x = WIDTH / 2;
        y = HEIGHT / 2;
        score = 0;
        tailLength = 3;
        for (int i = 0; i < tailLength; i++) {
            tailX[i] = x - (i + 1);
            tailY[i] = y;
        }
        spawnFood();
    }

    void draw() {
        system(CLEAR_SCREEN);
        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (j == 0) cout << "#";

                if (i == y && j == x)
                    cout << "O";
                else if (i == foodY && j == foodX)
                    cout << "F";
                else {
                    bool isTail = false;
                    for (int k = 0; k < tailLength; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o";
                            isTail = true;
                        }
                    }
                    if (!isTail) cout << " ";
                }

                if (j == WIDTH - 1) cout << "#";
            }
            cout << endl;
        }

        for (int i = 0; i < WIDTH + 2; i++) cout << "#";
        cout << endl;

        cout << "Score: " << score << " | High Score: " << highScore << endl;
        cout << "Controls: W/A/S/D to move, X to quit" << endl;
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'w': dir = (dir != DOWN) ? UP : DOWN; break;
                case 's': dir = (dir != UP) ? DOWN : UP; break;
                case 'a': dir = (dir != RIGHT) ? LEFT : RIGHT; break;
                case 'd': dir = (dir != LEFT) ? RIGHT : LEFT; break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
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

        if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
            gameOver = true;

        for (int i = 4; i < tailLength; i++) {
            if (tailX[i] == x && tailY[i] == y) {
                gameOver = true;
            }
        }

        if (x == foodX && y == foodY) {
            score += 10;
            if (score > highScore) {
                highScore = score;
            }
            tailLength++;
            spawnFood();
        }
    }

    bool isGameOver() { return gameOver; }

    void gameOverScreen() {
        cout << "\nGAME OVER! Your Final Score: " << score << endl;
        cout << "High Score: " << highScore << endl;
        cout << "Press R to Restart, X to Exit: ";

        char choice;
        do {
            choice = _getch();
        } while (choice != 'r' && choice != 'x');

        if (choice == 'r') {
            resetGame();
        }
    }
};

int SnakeGame::highScore = 0;

int main() {
    srand(time(0));

    char c;
    char diff;
    cout << "Select difficulty level\n1) Easy \n2) Medium \n3) Hard\n";
    cin >> diff;

    if (diff != '1' && diff != '2' && diff != '3') {
        cout << "Enter a proper difficulty level!" << endl;
        return 0;
    }

    do {
        SnakeGame game;
        while (!game.isGameOver()) {
            game.draw();
            game.input();
            game.logic();
            timedelay(diff == '1' ? 200 : diff == '2' ? 100 : 30);
        }
        game.draw();
        game.gameOverScreen();
        cin >> c;
    } while (c == 'R' || c == 'r');

    return 0;
}
