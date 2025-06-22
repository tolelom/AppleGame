#include "AppleGame.h"
#include <iostream>
#include <map>
#include <utility>

using namespace std;

AppleGame::AppleGame() = default;

AppleGame::~AppleGame() = default;

void AppleGame::InitBoard(vector<vector<int> > board) {
    this->board = std::move(board);

    rows = this->board.size();
    cols = this->board[0].size();
}

void AppleGame::Run() {
    cout << "Solve Apple Game" << endl;

    memo.clear();
    searchNumber = 0;
    dpNumber = 0;

    bitset<100> state;
    int result = Solve(state);

    cout << "result: " << result << " search number: " << searchNumber << " dp number: " << dpNumber << endl;
}

int AppleGame::Solve(bitset<100> state) {
    if (searchNumber % 100'000 == 0)
        cout << "searchNumber: " << searchNumber << endl;
    searchNumber++;

    if (memo.contains(state)) {
        dpNumber++;
        return memo[state];
    }


    int result = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result = max(result, calculate(i, j, i, j, state));
        }
    }

    memo[state] = result;

    return result;
}

int AppleGame::calculate(int y1, int x1, int y2, int x2, bitset<100> state) {
    int s = square(y1, x1, y2, x2, state);
    if (s > 10) return 0;


    if (s == 10) {
        auto [newState, removeCount] = remove(y1, x1, y2, x2, state);
        return removeCount + Solve(newState);
    }

    int result = 0;
    // case 1 (아래로 확장)
    if (y2 + 1 < board.size()) {
        result = max(result, calculate(y1, x1, y2 + 1, x2, state));
    }

    // case 2 (오른쪽으로 확장)
    if (x2 + 1 < board[0].size()) {
        result = max(result, calculate(y1, x1, y2, x2 + 1, state));
    }
    return result;
}


int AppleGame::square(int y1, int x1, int y2, int x2, bitset<100> state) const {
    int result = 0;
    for (int i = min(y1, y2); i <= max(y1, y2); ++i) {
        for (int j = min(x1, x2); j <= max(x1, x2); ++j) {
            if (!state.test(i * cols + j))
                result += board[i][j];
        }
    }
    return result;
}

pair<bitset<100>, int> AppleGame::remove(int y1, int x1, int y2, int x2, bitset<100> state) const {
    bitset<100> resultState = state;
    int count = 0;
    for (int i = min(y1, y2); i <= max(y1, y2); ++i) {
        for (int j = min(x1, x2); j <= max(x1, x2); ++j) {
            if (resultState.test(i * cols + j)) continue;
            resultState.set(i * cols + j);
            count++;
        }
    }
    return {resultState, count};
}

void AppleGame::PrintBoard(vector<vector<int> > &board) {
    for (auto &row: board) {
        for (int num: row) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    cout << endl;
}
