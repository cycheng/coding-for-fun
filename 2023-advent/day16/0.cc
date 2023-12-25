// https://adventofcode.com/2023/day/16

#include <bits/stdc++.h>
using namespace std;

/* format
.|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|....
*/

using Graph = vector<string>;
using DirGraph = vector<vector<uint8_t>>;

enum {
  north = 1 << 0,
  west = 1 << 1,
  south = 1 << 2,
  east = 1 << 3,
};

pair<int, int> nextMove(int row, int col, uint8_t dir) {
  switch (dir) {
  case north: return {row - 1, col};
  case south: return {row + 1, col};
  case east: return {row, col + 1};
  case west: return {row, col - 1};
  }
  assert(false);
  return {0, 0};
}

void dfs(Graph &graph, DirGraph &dirg, int row, int col, uint8_t dir) {
  // out of bound
  if (row < 0 || col < 0 || row == (int)graph.size() ||
      col == (int)graph[0].size())
    return;

  // has been visited.
  if (dirg[row][col] & dir)
    return;

  // get next dir:
  char sym = graph[row][col];
  dirg[row][col] |= dir;

  switch (sym) {
  case '.': {
    break;
  }
  case '|': {
    if (dir & (west | east))
      dir = north | south;
    break;
  }
  case '-': {
    if (dir & (north | south))
      dir = west | east;
    break;
  }
  case '\\': {
    switch (dir) {
    case east: dir = south; break;
    case west: dir = north; break;
    case north: dir = west; break;
    case south: dir = east; break;
    }
    break;
  }
  case '/': {
    switch (dir) {
    case east: dir = north; break;
    case west: dir = south; break;
    case north: dir = east; break;
    case south: dir = west; break;
    }
    break;
  }
  default: assert(false);
  }

  for (int i = 0; i < 4; ++i) {
    int newdir = 1 << i;
    if (dir & newdir) {
      auto next = nextMove(row, col, newdir);
      dfs(graph, dirg, next.first, next.second, newdir);
    }
  }
}

uint32_t solve(Graph &graph) {
  DirGraph dgraph(graph.size(), vector<uint8_t>(graph[0].length(), 0));
  dfs(graph, dgraph, 0, 0, east);

  uint32_t ans = 0;
  for (auto &row : dgraph)
    for (auto v : row)
      if (v)
        ++ans;
  return ans;
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  Graph graph;
  for (std::string line; getline(input, line);)
    graph.emplace_back(line);

  cout << "ans = " << solve(graph) << "\n";

  return 0;
}
