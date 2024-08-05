//Declaraciones y Estructuras
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>
#include <string>
#include <fstream>
#include <algorithm>

using namespace std;

const int SIZE = 4;
const string SCORE_FILE = "scores.txt";

struct Player {
    string name;
    int score;
};

//Inicialización y Añadir Azulejo
void initializeBoard(vector<vector<int>>& board) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = 0;
        }
    }
}

void addRandomTile(vector<vector<int>>& board) {
    vector<pair<int, int>> emptyTiles;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyTiles.push_back(make_pair(i, j));
            }
        }
    }

    if (!emptyTiles.empty()) {
        int index = rand() % emptyTiles.size();
        int value = (rand() % 2 + 1) * 2;
        board[emptyTiles[index].first][emptyTiles[index].second] = value;
    }
}

//Imprimir el Tablero y Verificar Movimientos
void printBoard(const vector<vector<int>>& board) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            cout << setw(5) << board[i][j];
        }
        cout << endl;
    }
}

bool canMove(const vector<vector<int>>& board) {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (board[i][j] == 0) return true;
            if (i > 0 && board[i][j] == board[i - 1][j]) return true;
            if (i < SIZE - 1 && board[i][j] == board[i + 1][j]) return true;
            if (j > 0 && board[i][j] == board[i][j - 1]) return true;
            if (j < SIZE - 1 && board[i][j] == board[i][j + 1]) return true;
        }
    }
    return false;
}

//Deslizar y Combinar Filas
int slideAndMergeRow(vector<int>& row) {
    int lastMergedIndex = -1;
    int score = 0;
    for (int i = 1; i < SIZE; ++i) {
        if (row[i] == 0) continue;
        int j = i;
        while (j > 0 && row[j - 1] == 0) {
            row[j - 1] = row[j];
            row[j] = 0;
            --j;
        }
        if (j > 0 && row[j - 1] == row[j] && lastMergedIndex != j - 1) {
            row[j - 1] *= 2;
            score += row[j - 1];
            row[j] = 0;
            lastMergedIndex = j - 1;
        }
    }
    return score;
}

//Movimientos del Tablero
int moveLeft(vector<vector<int>>& board) {
    int score = 0;
    for (int i = 0; i < SIZE; ++i) {
        score += slideAndMergeRow(board[i]);
    }
    return score;
}

void rotateBoard(vector<vector<int>>& board) {
    vector<vector<int>> temp = board;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            board[i][j] = temp[j][SIZE - i - 1];
        }
    }
}

int moveRight(vector<vector<int>>& board) {
    rotateBoard(board);
    rotateBoard(board);
    int score = moveLeft(board);
    rotateBoard(board);
    rotateBoard(board);
    return score;
}

int moveUp(vector<vector<int>>& board) {
    rotateBoard(board);
    int score = moveLeft(board);
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    return score;
}

int moveDown(vector<vector<int>>& board) {
    rotateBoard(board);
    rotateBoard(board);
    rotateBoard(board);
    int score = moveLeft(board);
    rotateBoard(board);
    return score;
}

//Guardar y Cargar Puntajes
void saveScore(const Player& player) {
    ofstream outFile(SCORE_FILE, ios::app);
    if (outFile.is_open()) {
        outFile << player.name << " " << player.score << endl;
        outFile.close();
    } else {
        cerr << "Unable to open score file." << endl;
    }
}

void loadScores(vector<Player>& players) {
    ifstream inFile(SCORE_FILE);
    if (inFile.is_open()) {
        Player player;
        while (inFile >> player.name >> player.score) {
            players.push_back(player);
        }
        inFile.close();
    } else {
        cerr << "Unable to open score file." << endl;
    }
}

//Imprimir Puntajes y Jugar el Juego
void printScores(const vector<Player>& players) {
    cout << "Player Scores:" << endl;
    for (const auto& player : players) {
        cout << player.name << ": " << player.score << endl;
    }
}

void playGame() {
    vector<vector<int>> board(SIZE, vector<int>(SIZE, 0));
    initializeBoard(board);
    addRandomTile(board);
    addRandomTile(board);

    string playerName;
    cout << "Enter player name: ";
    cin >> playerName;

    char move;
    bool gameOver = false;
    int score = 0;

    while (!gameOver) {
        printBoard(board);
        cout << "Enter move (w/a/s/d): ";
        cin >> move;

        int gainedScore = 0;
        switch (move) {
            case 'w':
                gainedScore = moveUp(board);
                break;
            case 'a':
                gainedScore = moveLeft(board);
                break;
            case 's':
                gainedScore = moveDown(board);
                break;
            case 'd':
                gainedScore = moveRight(board);
                break;
            default:
                cout << "Invalid move. Try again." << endl;
                continue;
        }

        score += gainedScore;

        if (!canMove(board)) {
            gameOver = true;
            cout << "Game over!" << endl;
        } else {
            addRandomTile(board);
        }
    }

    Player player = {playerName, score};
    saveScore(player);
    cout << "Your score: " << score << endl;
}

void showCredits() {
    cout << "Credits:" << endl;
    cout << "Michael Escobar" << endl;
    cout << "Jarvin Rios" << endl;
}

// Menú Principal
void showMenu() {
    cout << "Menu Options:" << endl;
    cout << "1. Play" << endl;
    cout << "2. Show Scores" << endl;
    cout << "3. Credits" << endl;
    cout << "4. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    srand(time(0));
    vector<Player> players;

    bool exit = false;
    int choice;

    while (!exit) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                playGame();
                break;
            case 2:
                players.clear();
                loadScores(players);
                printScores(players);
                break;
            case 3:
                showCredits();
                break;
            case 4:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    }

    return 0;
}