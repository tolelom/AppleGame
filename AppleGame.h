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
    std::unordered_map<std::bitset<64>, int> memo;

    long long searchNumber;
    long long dpNumber;

    int cols;
    int rows;

    void PrintBoard(const std::vector<std::vector<int> >& board);

    int Solve(std::bitset<64> state);

    int square(int y1, int x1, int y2, int x2, std::bitset<64> state);

    std::pair<std::bitset<64>, int> remove(int y1, int x1, int y2, int x2, std::bitset<64> state);

    int calculate(int y1, int x1, int y2, int x2, std::bitset<64> state);

    std::bitset<64> stateToMask(const std::vector<std::vector<int> >& board);
};


#endif //APPLEGAME_H
