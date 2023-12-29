// https://adventofcode.com/2023/day/21

#include <bits/stdc++.h>
using namespace std;

using Graph = vector<string>;
using Pos = pair<int, int>;

enum { X = 0, Y };

uint64_t xyhash(int x, int y) { return x + ((uint64_t)y << 32); }

void solve(Graph &graph, Pos &start, int step) {
  queue<Pos> bfsq;
  bfsq.push(start);
  int counter = 0;
  for (; step != 0; --step) {
    int len = (int)bfsq.size();
    unordered_set<uint64_t> visited;
    for (int i = 0; i < len; ++i) {
      Pos &p = bfsq.front();
      bfsq.pop();
      int x = p.second, y = p.first;
      if (visited.count(xyhash(x, y)))
        continue;
      visited.insert(xyhash(x, y));
      if (step == 1)
        ++counter;
      if (x > 0 && graph[y][x - 1] != '#')
        bfsq.emplace(x - 1, y);
      if (x < (int)graph[0].length() - 1 && graph[y][x + 1] != '#')
        bfsq.emplace(x + 1, y);
      if (y > 0 && graph[y - 1][x] != '#')
        bfsq.emplace(x, y - 1);
      if (y < (int)graph.size() - 1 && graph[y + 1][x] != '#')
        bfsq.emplace(x, y + 1);
    }
  }
  cout << "ans = " << counter << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  Graph graph;
  for (std::string line; getline(input, line);) {
    graph.push_back(line);
  }
  Pos start;
  for (int i = 0; i < (int)graph.size(); ++i)
    for (int j = 0; j < (int)graph[0].length(); ++j)
      if (graph[i][j] == 'S') {
        start = {i, j};
      }

  solve(graph, start, 64 + 1);
  return 0;
}
