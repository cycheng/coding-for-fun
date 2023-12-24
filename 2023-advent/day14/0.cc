// https://adventofcode.com/2023/day/13

#include <bits/stdc++.h>
using namespace std;

/* format

O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....

*/

using Graph = vector<string>;

// <column, row<which_row, number rocks>>
using Counts = vector<vector<pair<uint32_t, uint32_t>>>;

Counts rockCounts;

void printGraph(Graph &graph) {
  for (auto &str : graph) {
    cout << str << "\n";
  }
}

uint32_t solve(Graph &graph) {
  uint32_t numColumn = graph[0].length();
  rockCounts.resize(numColumn);
  for (uint32_t col = 0; col < numColumn; ++col)
    rockCounts[col].emplace_back(0, 0);
  for (uint32_t row = 0; row < graph.size(); ++row) {
    for (uint32_t col = 0; col < numColumn; ++col) {
      if (graph[row][col] == 'O') {
        rockCounts[col].back().second += 1;
      } else if (graph[row][col] == '#') {
        rockCounts[col].emplace_back(row + 1, 0);
      }
    }
  }

  uint32_t numRow = graph.size();
  uint32_t ans = 0;
  for (uint32_t col = 0; col < numColumn; ++col) {
    for (auto &rowCnt : rockCounts[col]) {
      if (rowCnt.second) {
        int32_t upper = numRow - rowCnt.first;
        int32_t lower = upper - (rowCnt.second - 1);
        int32_t hight = rowCnt.second;
        int32_t cnt = (upper + lower) * hight / 2;
        assert(((upper + lower) * hight) % 2 == 0);
        cout << rowCnt.first << ", " << col << " = " << rowCnt.second << " = "
             << cnt << "\n";
        ans += cnt;
      }
    }
  }
  return ans;
}

// https://github.com/TSoli/advent-of-code/blob/main/2023/day14a/solution.cpp
int getLoad(const std::vector<std::string> &map) {
  std::vector<int> maxPos(map[0].size(), map.size());
  int load{0};

  for (int i = 0; i < map.size(); ++i) {
    for (int j = 0; j < map[i].size(); ++j) {
      char square{map[i][j]};
      switch (square) {
      case 'O': load += maxPos[j]--; break;
      case '#': maxPos[j] = map.size() - i - 1; break;
      }
    }
  }

  return load;
}
constexpr int NumExpansion = (5 - 1);
// constexpr int NumExpansion = (1 - 1);

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  Graph graph;
  for (std::string line; getline(input, line);) {
    graph.emplace_back(line);
  }
  sum += solve(graph);
  cout << "ans = " << sum << "\n";

  return 0;
}
