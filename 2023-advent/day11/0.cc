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

// const uint32_t ExpansionNum = (10 - 1);
const uint32_t ExpansionNum = (2 - 1);

void expand() {
  vector<int32_t> rowNeedExpand, colNeedExpand;
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

  sort(rowNeedExpand.begin(), rowNeedExpand.end(), std::greater<int32_t>());
  sort(colNeedExpand.begin(), colNeedExpand.end(), std::greater<int32_t>());

  for (int32_t col : colNeedExpand) {
    for (auto &row : Graph)
      for (uint32_t i = 0; i < ExpansionNum; ++i)
        row.insert(row.begin() + col, '.');
  }

  for (int32_t row : rowNeedExpand)
    for (uint32_t i = 0; i < ExpansionNum; ++i)
      Graph.insert(Graph.begin() + row, string(Graph[0].size(), '.'));

  for (auto &row : Graph) {
    for (char c : row)
      cout << c;
    cout << "\n";
  }
}

void solve() {
  for (size_t i = 0; i < Graph.size(); ++i) {
    for (size_t j = 0; j < Graph[0].size(); ++j) {
      if (Graph[i][j] == '#')
        Galaxies.emplace_back(i, j);
    }
  }

  uint32_t ans = 0;
  for (size_t i = 0; i < Galaxies.size() - 1; ++i) {
    for (size_t j = i + 1; j < Galaxies.size(); ++j) {
      int32_t rdiff = abs(Galaxies[i].row - Galaxies[j].row);
      int32_t cdiff = abs(Galaxies[i].column - Galaxies[j].column);
      cout << Galaxies[i].row << " " << Galaxies[i].column << ", "
           << Galaxies[j].row << " " << Galaxies[j].column << ": ";
      cout << rdiff + cdiff << "\n";
      ans += rdiff + cdiff;
    }
  }

  cout << "ans = " << ans << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    Graph.push_back(line);
  }
  expand();
  solve();
  return 0;
}

/*
.....X..........
...........#....
#...............
................
................
................
..........#.....
.#..............
...............X
................
................
................
...........#....
#.....#.........

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