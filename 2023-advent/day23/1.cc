// https://adventofcode.com/2023/day/23

#include <bits/stdc++.h>
using namespace std;

using Graph = vector<string>;
using BoolGraph = vector<vector<bool>>;

// paths (.), forest (#), and steep slopes (^, >, v, and <).
//
// if you step onto a slope tile, your next step must be downhill (in the
// direction the arrow is pointing).
//
// How many steps long is the longest hike?

int MaxLen;

void dfs(const Graph &graph, int x, int y, int curLen, BoolGraph &visited) {
  int numrow = graph.size(), numcol = graph[0].length();
  if (x < 0 || y < 0 || x == numrow || y == numcol)
    return;

  char sym = graph[x][y];
  if (visited[x][y] || sym == '#')
    return;

  curLen += 1;
  if (x == numrow - 1) {
    MaxLen = max(MaxLen, curLen);
    return;
  }

  visited[x][y] = true;
  // brute force: 4m58.147s for my full input.
  dfs(graph, x - 1, y, curLen, visited);
  dfs(graph, x, y + 1, curLen, visited);
  dfs(graph, x + 1, y, curLen, visited);
  dfs(graph, x, y - 1, curLen, visited);
  visited[x][y] = false;
}

void solve(const Graph &graph) {
  BoolGraph visited(graph.size(), vector<bool>(graph[0].length()));
  int y = 0;
  for (; y < (int)graph[0].length(); ++y)
    if (graph[0][y] == '.')
      break;
  dfs(graph, 0, y, -1, visited);
  cout << "ans = " << MaxLen << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  Graph graph;
  for (std::string line; getline(input, line);) {
    graph.push_back(line);
  }
  solve(graph);
  return 0;
}