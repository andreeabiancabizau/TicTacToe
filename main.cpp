#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int PLAYER_X = 1;   // Human
const int PLAYER_O = -1;  // AI
const int EMPTY = 0;

vector<vector<int>> board(3, vector<int>(3, EMPTY)); //tabla 3 pe 3
bool aiFirst = false; // incepe aiul

void setStartingPlayer() {
    char choice;
    cout << "Do you want AI to start? (y/n): ";
    cin >> choice;
    aiFirst = (choice == 'y' || choice == 'Y'); //daca zice playerul da atunci incepe ai ul
}

// Function to check if the game is over
int checkWinner() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != EMPTY && board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0]; //variantele de castig
        if (board[0][i] != EMPTY && board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] != EMPTY && board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] != EMPTY && board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    
    for (const auto& row : board)
        for (int cell : row)
            if (cell == EMPTY)
                return 0;
    
    return 2;
}

// Minimax Algorithm
int minimax(int depth, bool isMaximizing) {
    int result = checkWinner();
    if (result != 0)
        return (result == PLAYER_O) ? 10 - depth : (result == PLAYER_X) ? depth - 10 : 0;
    
    int bestScore = isMaximizing ? numeric_limits<int>::min() : numeric_limits<int>::max();
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = isMaximizing ? PLAYER_O : PLAYER_X;
                int score = minimax(depth + 1, !isMaximizing);
                board[i][j] = EMPTY;
                bestScore = isMaximizing ? max(bestScore, score) : min(bestScore, score);
            }
        }
    }
    return bestScore;
}

// cea mai buna miscare a AI-ului 
pair<int, int> bestMove() {
    int bestScore = numeric_limits<int>::min();
    pair<int, int> move = {-1, -1};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == EMPTY) {
                board[i][j] = PLAYER_O;
                int score = minimax(0, false);
                board[i][j] = EMPTY;
                if (score > bestScore) {
                    bestScore = score;
                    move = {i, j};
                }
            }
        }
    }
    return move;
}

void printBoard() {
    cout << "\nBoard State:" << endl;
    for (const auto& row : board) {
        for (int cell : row) {
            if (cell == PLAYER_X) cout << " X ";
            else if (cell == PLAYER_O) cout << " O ";
            else cout << " . ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    setStartingPlayer();
    printBoard();
    if (aiFirst) {
        pair<int, int> move = bestMove();
        board[move.first][move.second] = PLAYER_O;
        cout << "AI chooses: (" << move.first << ", " << move.second << ")\n";
        printBoard();
    }

    int turn = PLAYER_X; // Player starts unless AI is first
    while (checkWinner() == 0) {
        int row, col;
      
        cout << "Enter your move (row and column): ";
        cin >> row >> col;
  
        if (board[row][col] == EMPTY) {
            board[row][col] = turn;
            printBoard();
            turn = -turn; // Switch turns

            if (turn == PLAYER_O) {
                pair<int, int> move = bestMove();
                board[move.first][move.second] = PLAYER_O;
                cout << "AI chooses: (" << move.first << ", " << move.second << ")\n";
                printBoard();
                turn = PLAYER_X;
            }
        } else {
            cout << "Invalid move! Try again.\n";
        }
    }

    int result = checkWinner();
    if (result == PLAYER_X)
        cout << "You win!\n";
    else if (result == PLAYER_O)
        cout << "AI wins!\n";
    else
        cout << "It's a draw!\n";

    return 0;
}

