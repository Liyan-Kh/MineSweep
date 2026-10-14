#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

using namespace std;

const char COVERED = '*';
const char FLAGGED = '^';
const char MINED = '!';

struct player {
    int no = 1;
    string username;
    int wins = 0;
    int loses = 0;
    double bestRecord = 0;
};

struct board {
    bool isMined = false;
    bool isRevealed = false;
    bool isFlagged = false;
    int nextToMines = 0;
};

void bombPutter(board **arr, int rows, int cols, int mines) {
    srand(time(0));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            arr[i][j].isMined = false;
            arr[i][j].isRevealed = false;
            arr[i][j].isFlagged = false;
            arr[i][j].nextToMines = 0;
        }
    }

    int placedMines = 0;
    while (placedMines < mines) {
        int row = rand() % rows;
        int col = rand() % cols;
        if (!arr[row][col].isMined) {
            arr[row][col].isMined = true;
            placedMines++;
        }
    }
}

void checker(board **arr, int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!arr[i][j].isMined) {
                int bCount = 0;
                for (int k = -1; k < 2; ++k) {
                    for (int l = -1; l < 2; ++l) {
                        int ni = i + k, nj = j + l;
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && arr[ni][nj].isMined)
                            bCount++;
                    }
                }
                arr[i][j].nextToMines = bCount;
            }
        }
    }
}

void revealing(board **arr, int row, int col, int Rows, int Cols, int &revealCount) {
    if (row < 0 || row >= Rows || col < 0 || col >= Cols || arr[row][col].isRevealed)
        return;
    if (arr[row][col].isFlagged == true) {
        int state;
        cout << "the tale in row " << row + Rows << "and column " << col + 1 << " is flagged.\n"
                                                                                "Are you sure about revealing this tale?\n"
                                                                                "1. Yes\n"
                                                                                "2. No\n";
        cin >> state;
        if (state == 1) {
            arr[row][col].isRevealed = true;
            revealCount++;
        }


    } else {
        arr[row][col].isRevealed = true;
        revealCount++;
    }

    if (arr[row][col].nextToMines == 0 && arr[row][col].isMined == false) {
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i != 0 || j != 0)
                    revealing(arr, row + i, col + j, Rows, Cols, revealCount);
            }
        }
    }
}

void displayBoard(board **table, int ROWS, int COLS) {

    for (int row = 0; row < ROWS; ++row) {
        cout << ROWS - row << " ";
        for (int col = 0; col < COLS; ++col) {
            if (!table[row][col].isRevealed) {
                if (table[row][col].isFlagged) {
                    cout << FLAGGED << " ";
                } else {
                    cout << COVERED << " ";
                }
            } else {
                if (table[row][col].isMined) {
                    cout << MINED << " ";
                } else if (table[row][col].nextToMines == 0) {
                    cout << "-" << " ";
                } else
                    cout << table[row][col].nextToMines << " ";
            }
        }
        cout << endl;
    }
    cout << "  ";
    for (int col = 1; col <= COLS; ++col) {
        cout << col << " ";
    }
    cout << endl;
}

void userInput(int &row, int &col, int &action, int maxRow, int maxCol) {
    cout << "What do you want to do?\n"
         << "1. Reveal a tile\n"
         << "2. Place a flag\n"
         << "3. Remove a flag\n"
         << "4. Exit\n";
    cin >> action;

    if (action == 1 || action == 2 || action == 3) {
        while (true) {
            cout << "Enter your chosen row:";
            cin >> row;
            cout << "Enter your chosen column:";
            cin >> col;
            if (row > 0 && row <= maxRow && col > 0 && col <= maxCol)
                break;
            else
                cout << "Invalid coordinates! Try again." << endl;
        }


        row = maxRow - row;
        col--;
    }
}

void processInput(board **table, int row, int col, int action, int Rows, int Cols, int &revealCount) {
    if (action == 1) {
        if (!table[row][col].isRevealed) {
            revealing(table, row, col, Rows, Cols, revealCount);
        }
    } else if (action == 2) {
        if (!table[row][col].isRevealed && !table[row][col].isFlagged) {
            table[row][col].isFlagged = true;
        }
    } else if (action == 3) {
        if (table[row][col].isFlagged) {
            table[row][col].isFlagged = false;
        }
    }
}

void readFromFile(player *Players, int &numOfPlayer) {
    ifstream inFile("tableScore.txt");
    if (inFile.is_open()) {
        string line;
        getline(inFile, line);

        numOfPlayer = 0;
        while (getline(inFile, line) && numOfPlayer < 100) {
            istringstream row(line);
            row >> Players[numOfPlayer].no >> Players[numOfPlayer].username >> Players[numOfPlayer].wins
                >> Players[numOfPlayer].loses >> Players[numOfPlayer].bestRecord;
            numOfPlayer++;
        }

        inFile.close();
    } else {
        cerr << "Unable to open file. You should make one!" << endl;
        numOfPlayer = 0;
    }
}

void login(player *p, string &userName, char result, int &numOfPlayer, int &n, double duration) {
    for (int i = 0; i < n; ++i) {
        if (p[i].username == userName) {
            if (result == 'w')
                p[i].wins++;
            else if (result == 'l')
                p[i].loses++;
            if (duration < p[numOfPlayer].bestRecord)
                p[numOfPlayer].bestRecord = duration;
            return;
        }
    }
    p[numOfPlayer].username = userName;
    p[numOfPlayer].bestRecord = duration;
    if (result == 'w')
        p[numOfPlayer].wins++;
    else if (result == 'l')
        p[numOfPlayer].loses++;
    numOfPlayer++;

}

void tableScore(player *p, int n) {
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (p[j].wins < p[j + 1].wins) {
                player temp = p[j];
                p[j] = p[j + 1];
                p[j + 1] = temp;
            }
            if (p[j].wins == p[j + 1].wins) {
                if (p[j].loses > p[j + 1].loses) {
                    player temp = p[j];
                    p[j] = p[j + 1];
                    p[j + 1] = temp;
                }

            }
            if (p[j].loses == p[j + 1].loses) {
                if (p[j].bestRecord > p[j + 1].bestRecord) {
                    player temp = p[j];
                    p[j] = p[j + 1];
                    p[j + 1] = temp;
                }
            }
        }
    }
    cout << left << setw(5) << "No."
         << setw(15) << "Name"
         << setw(10) << "Wins"
         << setw(10) << "Losses"
         << setw(15) << "Best Record" << endl;

    for (int i = 0; i < n; ++i) {
        cout << left << setw(5) << (i + 1)
             << setw(15) << p[i].username
             << setw(10) << p[i].wins
             << setw(10) << p[i].loses
             << setw(15) << p[i].bestRecord << endl;
    }
}

int main() {
    system("Color 70");
    int order = 0;
    int rows, cols, mines, revealCount = 0, n = 100, numOfPlayer = 0;
    player *Players;
    Players = new player[n];
    readFromFile(Players, numOfPlayer);
    while (order != 3) {

        cout << "Welcome to MineSweeper!" << endl
             << "What would you like to do?" << endl
             << "1. Start a new game\n"
             << "2. See the score table\n"
             << "3. Exit\n";
        cin >> order;


        char result;
        if (order == 1) {
            int level;
            cout << "Choose the complexity level:\n"
                 << "1. Beginner\n"
                 << "2. Intermediate\n"
                 << "3. Hard\n"
                 << "4. Custom\n";
            cin >> level;
            board **table;
            string userName;
            switch (level) {
                case 1:
                    rows = 9;
                    cols = 9;
                    mines = 10;
                    break;
                case 2:
                    rows = 16;
                    cols = 16;
                    mines = 40;
                    break;
                case 3:
                    rows = 16;
                    cols = 30;
                    mines = 99;
                    break;
                case 4:
                    cout << "Enter custom board size and number of mines:\n";
                    cout << "Rows:";
                    cin >> rows;
                    cout << "Columns:";
                    cin >> cols;
                    cout << "Mines:";
                    cin >> mines;
                    break;
                default:
                    continue;
            }
            table = new board *[rows];
            for (int i = 0; i < rows; ++i) {
                table[i] = new board[cols];
            }


            bombPutter(table, rows, cols, mines);
            checker(table, rows, cols);
            clock_t t;
            t = clock();

            while (true) {
                displayBoard(table, rows, cols);
                int row, col, action;
                userInput(row, col, action, rows, cols);

                if (action == 4) {
                    cout << "Exiting game.\n";
                    break;
                }
                processInput(table, row, col, action, rows, cols, revealCount);
                if (table[row][col].isMined && action == 1) {
                    displayBoard(table, rows, cols);
                    t = clock() - t;
                    double timeTaken = (double(t)) / CLOCKS_PER_SEC;
                    cout << "Boom! Game over.\n"
                            "tell me your username to count you in:\n"
                            "your time: " << timeTaken << endl;
                    cin >> userName;
                    result = 'l';
                    login(Players, userName, result, numOfPlayer, n, timeTaken);
                    revealCount = 0;
                    break;
                }
                if (revealCount == (rows * cols) - mines) {
                    displayBoard(table, rows, cols);
                    t = clock() - t;
                    double timeTaken = (double(t)) / CLOCKS_PER_SEC;
                    cout << "Wow! You won!\n"
                            "tell me your username to count you in:\n"
                            "Your Time: " << timeTaken << endl;
                    cin >> userName;
                    result = 'w';
                    login(Players, userName, result, numOfPlayer, n, timeTaken);
                    revealCount = 0;
                    break;

                }
            }

            for (int i = 0; i < rows; ++i) {
                delete[] table[i];
            }
            delete[] table;
        }
        if (order == 2) {
            tableScore(Players, numOfPlayer);
            ofstream outFile("tableScore.txt");
            if (outFile.is_open()) {
                outFile << left << setw(5) << "No."
                        << setw(15) << "Name"
                        << setw(10) << "Wins"
                        << setw(10) << "Losses"
                        << setw(15) << "Best Record" << endl;

                // Write the player data
                for (int i = 0; i < numOfPlayer; ++i) {
                    outFile << left << setw(5) << (i + 1)
                            << setw(15) << Players[i].username
                            << setw(10) << Players[i].wins
                            << setw(10) << Players[i].loses
                            << setw(15) << Players[i].bestRecord << endl;
                }

                outFile.close();
            } else {
                cerr << "Unable to open file" << endl;
            }
        }
    }
    return 0;
}



