#ifndef APPLEGAME_H
#define APPLEGAME_H

#include <bitset>
#include <map>
#include <unordered_map>
#include <vector>


class AppleGame {
public:
    AppleGame();

    ~AppleGame();

    void InitBoard(std::vector<std::vector<int> > board);

    void Run();

private:
    std::vector<std::vector<int> > board;
    std::unordered_map<std::bitset<100>, int> memo;
    std:: vector<int> flat_board; // 1D 배열로 변경

    long long searchNumber;
    long long dpNumber;

    int cols;
    int rows;

    void PrintBoard(std::vector<std::vector<int> > &board);

    int Solve(const std::bitset<100>& state);

    int square(int y1, int x1, int y2, int x2, const std::bitset<100>& state) const;

    std::pair<std::bitset<100>, int> remove(int y1, int x1, int y2, int x2, const std::bitset<100>& state) const;

    int calculate(int y1, int x1, int y2, int x2, const std::bitset<100>& state, bool direction);
};


#endif //APPLEGAME_H
