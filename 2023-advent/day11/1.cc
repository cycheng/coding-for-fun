// https://adventofcode.com/2023/day/7

#include <bits/stdc++.h>
using namespace std;

/* format
...#......
.......#..
#.........
..........
......#...
.#........
.........#
..........
.......#..
#...#.....
*/

int32_t Invalid = 0xffffffff;
struct Pos {
  int32_t row = Invalid, column = 0;
  Pos() = default;
  Pos(int32_t r, int32_t c) : row(r), column(c) {}
  bool valid() { return row != Invalid; }
  bool operator==(const Pos &p) const {
    return p.row == row && p.column == column;
  }
};

vector<string> Graph;
vector<Pos> Galaxies;
vector<int32_t> rowNeedExpand, colNeedExpand;

void computeExpansion() {
  for (size_t i = 0; i < Graph.size(); ++i)
    if (Graph[i].find('#') == string::npos)
      rowNeedExpand.push_back(i);

  for (size_t i = 0; i < Graph[0].size(); ++i) {
    bool hasSharp = false;
    for (size_t j = 0; j < Graph.size(); ++j) {
      if (Graph[j][i] == '#') {
        hasSharp = true;
        break;
      }
    }
    if (!hasSharp)
      colNeedExpand.push_back(i);
  }

  sort(rowNeedExpand.begin(), rowNeedExpand.end());
  sort(colNeedExpand.begin(), colNeedExpand.end());
}

const uint32_t ExpansionNum = (1000000 - 1);

uint32_t getRowExpansionCounts(uint32_t row) {
  auto upperIter = upper_bound(rowNeedExpand.begin(), rowNeedExpand.end(), row);
  uint32_t sum = 0;
  for (auto iter = rowNeedExpand.begin(); iter != upperIter; ++iter)
    ++sum;
  return sum;
}

uint32_t getColExpansionCounts(uint32_t col) {
  auto upperIter = upper_bound(colNeedExpand.begin(), colNeedExpand.end(), col);
  uint32_t sum = 0;
  for (auto iter = colNeedExpand.begin(); iter != upperIter; ++iter)
    ++sum;
  return sum;
}

void solve() {
  for (size_t i = 0; i < Graph.size(); ++i) {
    for (size_t j = 0; j < Graph[0].size(); ++j) {
      if (Graph[i][j] == '#')
        Galaxies.emplace_back(i, j);
    }
  }

  uint64_t ans = 0;
  for (size_t i = 0; i < Galaxies.size() - 1; ++i) {
    for (size_t j = i + 1; j < Galaxies.size(); ++j) {
      uint64_t riEC = getRowExpansionCounts(Galaxies[i].row);
      uint64_t rjEC = getRowExpansionCounts(Galaxies[j].row);
      uint64_t ciEC = getColExpansionCounts(Galaxies[i].column);
      uint64_t cjEC = getColExpansionCounts(Galaxies[j].column);
      if (riEC)
        riEC = riEC * ExpansionNum;
      if (rjEC)
        rjEC = rjEC * ExpansionNum;
      if (ciEC)
        ciEC = ciEC * ExpansionNum;
      if (cjEC)
        cjEC = cjEC * ExpansionNum;

      if (riEC > rjEC)
        swap(riEC, rjEC);
      if (ciEC > cjEC)
        swap(ciEC, cjEC);

      int32_t rdiff = abs(Galaxies[i].row - Galaxies[j].row);
      int32_t cdiff = abs(Galaxies[i].column - Galaxies[j].column);
      // cout << Galaxies[i].row << " " << Galaxies[i].column << ", "
      //      << Galaxies[j].row << " " << Galaxies[j].column << ": ";
      // cout << rdiff + cdiff + (rjEC - riEC) + (cjEC - ciEC) << "\n";
      ans += rdiff + cdiff + (rjEC - riEC) + (cjEC - ciEC);
    }
  }

  cout << "ans = " << ans << "\n";
}

/*
....#........
.........#...
#............
.............
.............
........#....
.#...........
............#
.............
.............
.........#...
#....#.......

...#......--- 0 3, 1 7: = 1 + 4 + (10)
.......#..
#.........
..........
......#...    0 3, 4 6: (26)
.#........
.........#
..........
.......#..
#...#.....
*/

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    Graph.push_back(line);
  }
  computeExpansion();
  solve();
  return 0;
}
