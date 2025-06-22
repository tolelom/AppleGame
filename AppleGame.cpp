#include "AppleGame.h"
#include <iostream>
#include <map>
#include <utility>

using namespace std;

AppleGame::AppleGame() {
};

AppleGame::~AppleGame() {
};

void AppleGame::InitBoard(vector<vector<int> > board) {
    this->board = std::move(board);

    rows = this->board.size();
    cols = this->board[0].size();
}

void AppleGame::Run() {
    cout << "Running Apple Game" << endl;
    cout << "Initial Board " << endl;
    PrintBoard(board);

    // init
    memo.clear();
    searchNumber = 0;
    dpNumber = 0;

    bitset<64> state;
    int result = Solve(state);

    cout << "result: " << result << " search number: " << searchNumber << " dp number: " << dpNumber << endl;
}

int AppleGame::Solve(bitset<64> state) {
    if (searchNumber % 100'000 == 0)
        cout << "searchNumber: " << searchNumber << endl;
    searchNumber++;

    if (memo.contains(state)) {
        dpNumber++;
        return memo[state];
    }


    int result = 0;
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[i].size(); j++) {
            result = max(result, calculate(i, j, i, j, state));
        }
    }

    memo[state] = result;

    return result;
}


int AppleGame::square(int y1, int x1, int y2, int x2, bitset<64> state) {
    int result = 0;
    for (int i = min(y1, y2); i <= max(y1, y2); ++i) {
        for (int j = min(x1, x2); j <= max(x1, x2); ++j) {
            if (!state.test(i * cols + j))
                result += board[i][j];
        }
    }
    return result;
}

pair<bitset<64>, int> AppleGame::remove(int y1, int x1, int y2, int x2, bitset<64> state) {
    bitset<64> resultState = state;
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

void AppleGame::PrintBoard(const vector<vector<int> > &board) {
    for (const auto &row: board) {
        for (int num: row) {
            std::cout << num << " ";
        }
        std::cout << std::endl;
    }
    cout << endl;
}

int AppleGame::calculate(int y1, int x1, int y2, int x2, bitset<64> state) {
    int s = square(y1, x1, y2, x2, state);
    if (s > 10) return 0;


    if (s == 10) {
        auto [newState, removeCount] = remove(y1, x1, y2, x2, state);
        return removeCount + Solve(newState);
    }

    if (s < 10) {
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
}


// Warning: 보드 가로 세로 곱이 64보다 크면 안됨
bitset<64> AppleGame::stateToMask(const std::vector<std::vector<int> > &board) {
    bitset<64> result = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] == 0) {
                result.set(i * cols + j);
            }
        }
    }

    return result;
}
