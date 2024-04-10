#include <iostream> // Standard input/output stream library
#include <vector> // Standard vector library
#include <algorithm> // Standard algorithm library
#include <random> // Standard random library
#include <ncurses.h> // Ncurses library for text-based user interfaces

using namespace std; // Using the standard namespace

const int rows = 10; // Define the number of rows in the maze
const int cols = 10; // Define the number of columns in the maze
const int wall = 1; // Define the value for a wall cell
const int empty = 0; // Define the value for an empty cell
const int start = 2; // Define the value for the starting cell
const int exitCell = 3; // Define the value for the exit cell

vector<vector<int> > maze(rows, vector<int>(cols, wall)); // Create a 2D vector to represent the maze with walls initially

int playerRow, playerCol; // Declare variables to store the player's position
int exitRow, exitCol; // Declare variables to store the exit's position

void printMaze() {
    clear(); // Clear the screen
    for (int i = 0; i < rows; ++i) { // Loop through each row of the maze
        for (int j = 0; j < cols; ++j) { // Loop through each column of the maze
            if (i == playerRow && j == playerCol) { // Check if the current cell is the player's position
                printw("P "); // Print 'P' to represent the player
            } else {
                switch (maze[i][j]) { // Check the value of the current cell
                    case wall:
                        printw("# "); // Print '#' to represent a wall
                        break;
                    case empty:
                        printw(". "); // Print '.' to represent an empty space
                        break;
                    case start:
                        printw("S "); // Print 'S' to represent the starting point
                        break;
                    case exitCell:
                        printw("E "); // Print 'E' to represent the exit
                        break;
                }
            }
        }
        printw("\n"); // Move to the next line
    }
    refresh(); // Refresh the screen to display changes
}

void generateMaze(int row, int col) {
    vector<int> directions; // Create a vector to store possible directions

    directions.push_back(1); // Add up direction to the vector
    directions.push_back(2); // Add right direction to the vector
    directions.push_back(3); // Add down direction to the vector
    directions.push_back(4); // Add left direction to the vector

    random_device rd; // Create a random device
    mt19937 gen(rd()); // Seed the random number generator
    uniform_int_distribution<> dis(0, 3); // Create a uniform distribution for random directions

    for (int i = 0; i < 4; ++i) { // Shuffle the directions vector
        int r = dis(gen) % 4;
        swap(directions[i], directions[r]);
    }

    for (int i = 0; i < 4; ++i) { // Loop through each direction
        int dir = directions[i]; // Get the current direction
        switch (dir) {
            case 1: // Up direction
                if (row - 2 <= 0) continue; // Check if moving up is within bounds
                if (maze[row - 2][col] != empty) { // Check if the cell above is not empty
                    maze[row - 2][col] = empty; // Set the cell above as empty
                    maze[row - 1][col] = empty; // Set the cell in between as empty
                    generateMaze(row - 2, col); // Recursively generate the maze from the new position
                }
                break;
            case 2: // Right direction
                if (col + 2 >= cols - 1) continue; // Check if moving right is within bounds
                if (maze[row][col + 2] != empty) { // Check if the cell to the right is not empty
                    maze[row][col + 2] = empty; // Set the cell to the right as empty
                    maze[row][col + 1] = empty; // Set the cell in between as empty
                    generateMaze(row, col + 2); // Recursively generate the maze from the new position
                }
                break;
            case 3: // Down direction
                if (row + 2 >= rows - 1) continue; // Check if moving down is within bounds
                if (maze[row + 2][col] != empty) { // Check if the cell below is not empty
                    maze[row + 2][col] = empty; // Set the cell below as empty
                    maze[row + 1][col] = empty; // Set the cell in between as empty
                    generateMaze(row + 2, col); // Recursively generate the maze from the new position
                }
                break;
            case 4: // Left direction
                if (col - 2 <= 0) continue; // Check if moving left is within bounds
                if (maze[row][col - 2] != empty) { // Check if the cell to the left is not empty
                    maze[row][col - 2] = empty; // Set the cell to the left as empty
                    maze[row][col - 1] = empty; // Set the cell in between as empty
                    generateMaze(row, col - 2); // Recursively generate the maze from the new position
                }
                break;
        }
    }
}

int main() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Don't echo characters to the screen
    keypad(stdscr, TRUE); // Enable keypad input

    maze[1][1] = start; // Set the starting point in the maze
    playerRow = 1; // Set the player's initial row position
    playerCol = 1; // Set the player's initial column position

    generateMaze(1, 1); // Generate the maze starting from the given position
    
    random_device rd; // Create a random device
    mt19937 gen(rd()); // Seed the random number generator
    uniform_int_distribution<> rowDis(1, rows - 2); // Create a uniform distribution for random rows
    uniform_int_distribution<> colDis(1, cols - 2); // Create a uniform distribution for random columns
    exitRow = rowDis(gen); // Generate a random row for the exit
    exitCol = colDis(gen); // Generate a random column for the exit
    maze[exitRow][exitCol] = exitCell; // Set the exit cell in the maze

    printMaze(); // Print the initial state of the maze

    char input; // Declare a variable to store user input
    while (true) { // Main game loop
        input = getch(); // Get user input

        switch (input) { // Process user input
            case 'w': // Move up
                if (maze[playerRow - 1][playerCol] != wall) { // Check if the move is valid
                    --playerRow; // Move the player up
                }
                break;
            case 'a': // Move left
                if (maze[playerRow][playerCol - 1] != wall) { // Check if the move is valid
                    --playerCol; // Move the player left
                }
                break;
            case 's': // Move down
                if (maze[playerRow + 1][playerCol] != wall) { // Check if the move is valid
                    ++playerRow; // Move the player down
                }
                break;
            case 'd': // Move right
                if (maze[playerRow][playerCol + 1] != wall) { // Check if the move is valid
                    ++playerCol; // Move the player right
                }
                break;
            default:
                break;
        }

        printMaze(); // Print the updated state of the maze

        if (playerRow == exitRow && playerCol == exitCol) { // Check if the player has reached the exit
            printw("Congratulations! You reached the end of the maze.\n"); // Print a message
            break; // Exit the game loop
        }
    }

    endwin(); // End ncurses

    return 0;
}
