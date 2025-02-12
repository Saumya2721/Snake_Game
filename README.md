# Snake_Game
# Snake Game

## Description
This is a simple console-based Snake game implemented in C++. The game allows the player to control a snake using the keyboard, eat food to grow in length, and avoid collisions with the walls and itself. The game includes three difficulty levels: Easy, Medium, and Hard, which affect the snake's speed.

## Table of Contents
- [Installation]
- [Usage]
- [Contributing]
- [License]
- [Features]
- [FAQ]
- [Acknowledgments]

## Installation
### Linux/macOS:
g++ snake_game.cpp -o snake_game

./snake_game
### Windows:
g++ snake_game.cpp -o snake_game.exe

snake_game.exe

## Usage
To run the project, execute the compiled binary:
./snake_game  # Linux/macOS

snake_game.exe  # Windows

### How to Play
1. Run the program.
2. Select a difficulty level (1: Easy, 2: Medium, 3: Hard).
3. Use the following controls to play:
   - `W` - Move Up
   - `A` - Move Left
   - `S` - Move Down
   - `D` - Move Right
   - `X` - Quit the game
   - `R` - Restart after Game Over
4. Avoid colliding with the walls and yourself.
5. Eat food (`F`) to grow and increase your score.
6. Try to beat the high score!

## Contributing
1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Make your changes.
4. Push your branch: `git push origin feature-name`.
5. Create a pull request.

## License
This project is licensed under the [MIT License](LICENSE).

## Features
- Simple gameplay using `W`, `A`, `S`, `D` keys for movement.
- Food spawning at random locations to increase score and snake length.
- Difficulty levels affecting game speed.
- Persistent high score.
- Cross-platform compatibility with Windows, Linux, and macOS.

## FAQ
**Q: Can I modify the game speed?**
A: Yes, by changing the `ms_speed` variable in the `SnakeGame` constructor.

**Q: How do I add new features?**
A: Fork the repo, create a new branch, make changes, and submit a pull request.

## Acknowledgments
- Inspired by the classic Snake game.
- Thanks to open-source contributors for ideas and improvements.

## Data Structures Used

### 1. **Arrays (`tailX`, `tailY`)**
   - **Purpose:** Stores the positions of the snake's tail.
   - **Usage:** The snake's body follows the movement of the head by shifting positions in the array.
   - **Efficiency:** Efficient for small games, but could be optimized using linked lists for dynamic memory management.

### 2. **Enum (`Direction`)**
   - **Purpose:** Represents the direction of the snake (UP, DOWN, LEFT, RIGHT, STOP).
   - **Usage:** Helps in managing movement logic cleanly without using arbitrary integers.
   - **Efficiency:** Improves code readability and avoids errors from using raw integer values.

### 3. **Primitive Variables (`x`, `y`, `foodX`, `foodY`)**
   - **Purpose:** Store the snake's head position and food position.
   - **Usage:** Used for movement updates and collision detection.
   - **Efficiency:** Simple and fast for storing fixed-position data.

### 4. **Static Variables (`highScore`)**
   - **Purpose:** Tracks the high score across game sessions.
   - **Usage:** Ensures score persistence while the program runs.
   - **Efficiency:** Avoids unnecessary memory allocation and global scope pollution.

### 5. **Boolean (`gameOver`)**
   - **Purpose:** Tracks whether the game is running or over.
   - **Usage:** Controls game loop execution.
   - **Efficiency:** Lightweight and efficient as a simple flag.

## Potential Improvements
- **Linked List for Snake Tail:** Using a linked list instead of an array would allow for dynamic memory allocation, preventing unnecessary memory usage for small snakes.
- **Dynamic Difficulty Adjustment:** Instead of fixed speed settings, the game could increase speed gradually as the snake grows.
- **Obstacle Addition:** Adding obstacles on the board would enhance difficulty and strategy.
- **Graphical Interface:** Replacing console output with a graphical library (like SDL or SFML) for better visuals.

## Badges
![Build Status](https://travis-ci.org/yourusername/yourproject.svg?branch=main)

## Conclusion
This Snake Game is a great demonstration of basic game mechanics using fundamental C++ programming concepts. The use of arrays, enums, and simple data structures makes it easy to understand and modify for further improvements.
