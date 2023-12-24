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

// <pattern, id>
using Patterns = unordered_map<string, uint32_t>;

void printGraph(Graph &graph) {
  for (auto &str : graph) {
    cout << str << "\n";
  }
}

enum {
  north,
  west,
  south,
  east,
};

// north, west, south, east
void moveRocks(Graph &graph, int dir) {
  // empty pos for a row or column
  vector<int> emptyPos;
  int posInit = 0, posLen = graph[0].length();
  switch (dir) {
  case north: break;
  case west: posLen = graph.size(); break;
  case south: posInit = posLen - 1; break;
  case east:
    posInit = posLen - 1;
    posLen = graph.size();
    break;
  default: break;
  }
  emptyPos.resize(posLen, posInit);

  int numRow = graph.size(), numCol = graph[0].length();
  // north, west:
  if (dir == north || dir == west) {
    for (int i = 0; i < numRow; ++i) {
      for (int j = 0; j < numCol; ++j) {
        char c = graph[i][j];
        if (c == '#') {
          if (dir == north)
            emptyPos[j] = i + 1;
          else
            emptyPos[i] = j + 1;
        } else if (c == 'O') {
          graph[i][j] = '.';
          if (dir == north) {
            graph[emptyPos[j]][j] = 'O';
            ++emptyPos[j];
          } else {
            graph[i][emptyPos[i]] = 'O';
            ++emptyPos[i];
          }
        }
      }
    }
  } else {
    // south, east:
    for (int i = numRow - 1; i >= 0; --i) {
      for (int j = numCol - 1; j >= 0; --j) {
        char c = graph[i][j];
        if (c == '#') {
          if (dir == south)
            emptyPos[j] = i - 1;
          else
            emptyPos[i] = j - 1;
        } else if (c == 'O') {
          graph[i][j] = '.';
          if (dir == south) {
            graph[emptyPos[j]][j] = 'O';
            --emptyPos[j];
          } else {
            graph[i][emptyPos[i]] = 'O';
            --emptyPos[i];
          }
        }
      }
    }
  }
}

string getPattern(Graph &graph) {
  string p;
  for (auto &s : graph)
    p += s;
  return p;
}

uint32_t countNorthLoads(Graph &graph) {
  int ans = 0;
  for (int i = 0; i < (int)graph.size(); ++i) {
    for (char c : graph[i]) {
      if (c == 'O')
        ans += graph.size() - i;
    }
  }
  return ans;
}

// 1000000000
uint32_t solve(Graph &graph, uint32_t cycles) {
  Patterns patterns;
  uint32_t pid = 0;
  bool foundRepeat = false;
  uint32_t dist = 0;
  uint32_t i, j;
  uint32_t orig = 0;
  for (i = 0; i < cycles && !foundRepeat; ++i) {
    for (j = 0; j < 4 && !foundRepeat; ++j) {
      moveRocks(graph, j);
      // cout << pid << " " << i << " " << j << " = " << countNorthLoads(graph)
      //      << "\n";
      string p = getPattern(graph);
      auto iter = patterns.find(p);
      if (iter != patterns.end()) {
        foundRepeat = true;
        dist = pid - iter->second;
        orig = iter->second;
        cout << "  dist pid orig " << dist << " " << pid << " " << iter->second
             << "\n";
      } else {
        patterns[p] = pid++;
      }
      // cout << "cycle " << i << " " << j << "\n";
      // printGraph(graph);
      // cout << "\n\n";
    }
  }

  if (foundRepeat) {
    int remainings = ((uint64_t)(cycles * 4) - orig) % (uint64_t)dist;
    // remainings = remainings % dist;
    --remainings;
    cout << "remainings = " << remainings << "\n";
    while (remainings > 0) {
      for (; j < 4 && remainings; ++j) {
        moveRocks(graph, j);
        --remainings;
        cout << j << " = " << countNorthLoads(graph) << "\n";
      }
      j = 0;
    }
  }
  return countNorthLoads(graph);
}

// https://github.com/TSoli/advent-of-code/blob/main/2023/day14a/solution.cpp
int getLoad(const std::vector<std::string> &map) {
  std::vector<int> maxPos(map[0].size(), map.size());
  int load{0};

  for (int i = 0; i < (int)map.size(); ++i) {
    for (int j = 0; j < (int)map[i].size(); ++j) {
      char square{map[i][j]};
      switch (square) {
      case 'O': load += maxPos[j]--; break;
      case '#': maxPos[j] = map.size() - i - 1; break;
      }
    }
  }

  return load;
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  Graph graph;
  for (std::string line; getline(input, line);) {
    graph.emplace_back(line);
  }
  sum += solve(graph, 1000000000);
  // sum += solve(graph, 3);
  cout << "ans = " << sum << "\n";

  return 0;
}
