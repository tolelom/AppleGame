#include "AppleGame.h"
#include <iostream>
#include <map>
#include <utility>
#include <omp.h>

using namespace std;

AppleGame::AppleGame() = default;

AppleGame::~AppleGame() = default;

void AppleGame::InitBoard(vector<vector<int> > board) {
    this->board = std::move(board);

    rows = this->board.size();
    cols = this->board[0].size();


    flat_board.clear();
    for (const auto& row : this->board) {
        flat_board.insert(flat_board.end(), row.begin(), row.end());
    }
}

void AppleGame::Run() {
    cout << "Solve Apple Game" << endl;

    for (auto it : flat_board)
        cout << it << ' ';
    cout << endl;

    memo.clear();
    searchNumber = 0;
    dpNumber = 0;

    bitset<100> state;
    int result = Solve(state);

    cout << "result: " << result << " search number: " << searchNumber << " dp number: " << dpNumber << endl;
}

int AppleGame::Solve(const bitset<100> &state) {
    if (memo.contains(state)) {
        dpNumber++;
        return memo[state];
    }

#pragma omp atomic
    searchNumber++;
    if (searchNumber % 100'000 == 0) {
#pragma omp critical
        cout << "searchNumber: " << searchNumber << endl;
    }

    int result = 0;

#pragma omp parallel for reduction(max:result) schedule(dynamic, 1)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // if (!state[i * cols + j]) {

            result = max(result, calculate(i, j, i, j, state, true));
            // }
        }
    }

#pragma omp critical
    memo[state] = result;

    return result;
}

int AppleGame::calculate(int y1, int x1, int y2, int x2, const bitset<100> &state, bool direction) {
    int s = square(y1, x1, y2, x2, state);
    if (s > 10) return 0;


    if (s == 10) {
        auto&& result = remove(y1, x1, y2, x2, state);
        return result.second + Solve(result.first);
    }

    int result = 0;
    // case 1 (아래로 확장)
    if (y2 + 1 < rows && direction) {
        result = max(result, calculate(y1, x1, y2 + 1, x2, state, true));
    }

    // case 2 (오른쪽으로 확장)
    if (x2 + 1 < cols) {
        result = max(result, calculate(y1, x1, y2, x2 + 1, state, false));
    }
    return result;
}


// // flat ver
// int AppleGame::square(int y1, int x1, int y2, int x2, const bitset<100>& state) const {
//     int result = 0;
//
//     for (int i = y1; i <= y2; ++i) {
//         const int row_start = i * cols + x1;  // 현재 행 시작 위치
//         const int row_end = i * cols + x2;    // 현재 행 끝 위치
//         for (int pos = row_start; pos <= row_end; ++pos) {
//             if (!state[pos]) {
//                 result += flat_board[pos];
//             }
//         }
//     }
//     return result;
// }

int AppleGame::square(int y1, int x1, int y2, int x2, const bitset<100> &state) const {
    int result = 0;
    for (int i = y1; i <= y2; ++i) {
        for (int j = x1; j <= x2; ++j) {
            if (!state[i * cols + j])
                result += board[i][j];
        }
    }
    return result;
}


pair<bitset<100>, int> AppleGame::remove(int y1, int x1, int y2, int x2, const bitset<100> &state) const {
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
