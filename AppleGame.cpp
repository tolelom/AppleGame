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

int AppleGame::Solve(const bitset<100>& state) {
    if (memo.contains(state)) {
        dpNumber++;
        return memo[state];
    }

    searchNumber++;
    if (searchNumber % 100'000 == 0)
        cout << "searchNumber: " << searchNumber << endl;

    int result = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result = max(result, calculate(i, j, i, j, state, true));
        }
    }

    memo[state] = result;

    return result;
}

int AppleGame::calculate(int y1, int x1, int y2, int x2, const bitset<100>& state, bool direction) {
    int s = square(y1, x1, y2, x2, state);
    if (s > 10) return 0;


    if (s == 10) {
        auto [newState, removeCount] = remove(y1, x1, y2, x2, state);
        return removeCount + Solve(newState);
    }

    int result = 0;
    // case 1 (아래로 확장)
    if (y2 + 1 < board.size() && direction) {
        result = max(result, calculate(y1, x1, y2 + 1, x2, state, true));
    }

    // case 2 (오른쪽으로 확장)
    if (x2 + 1 < board[0].size()) {
        result = max(result, calculate(y1, x1, y2, x2 + 1, state, false));
    }
    return result;
}


int AppleGame::square(int& y1, int& x1, int& y2, int& x2, const bitset<100>& state) const {
    int result = 0;
    for (int i = y1; i <= y2; ++i) {
        for (int j = x1; j <= x2; ++j) {
            if (!state[i * cols + j])
                result += board[i][j];
        }
    }
    return result;
}

pair<bitset<100>, int> AppleGame::remove(int& y1, int& x1, int& y2, int& x2, const bitset<100>& state) const {
    bitset<100> resultState = state;
    int count = 0;
    for (int i = y1; i <= y2; ++i) {
        for (int j = x1; j <= x2; ++j) {
            if (resultState[i * cols + j]) continue;
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
