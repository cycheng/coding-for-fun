// https://adventofcode.com/2023/day/21

#include <bits/stdc++.h>
using namespace std;

using Graph = vector<string>;
using Pos = pair<int, int>;

enum { X = 0, Y };

/*
for the full input:
step1:   step2:   step3:    step4:
                                 o
                      o         o o
            o        o o       o o o
  o        o o      o o o     o o o o
 oSo      o o o    o oSo o   o o o o o
  o        o o      o o o     o o o o
            o        o o       o o o
                      o         o o
                                 o

  4=2²    9=3²     16=4²      25=5²    ... (t+1)²
=> (@jmd-dk) It turns out that the entire perimeter of this diamond is exactly
reached after size/2 = 65 steps.

=> (@jmd-dk) Because the corner of the diamond are at the boundary of the map
(when thought of as non-periodic), and the middle row and column (where the
starting position S is located) are completely free (no rocks #), we are then
guaranteed that another 8 surrounding diamonds will be exactly reached after
size = 131 steps

=> (@jmd-dk) If we were in the continuous limit and with no rocks #, the number
of positions covered as a function of steps would be A(t) = πt² (area of disk),
where t is the number of steps.

=> (@jmd-dk) Having discrete steps and dismissing certain positions (adding in
rocks) cannot introduce higher-order terms, so the most general form will be
A(t) = at² + bt + c.

=> (@jmd-dk) We can determine a, b and c if we know A(t) for three values of t.
Here we can use
  t = size/2 = 65,
  t = size/2 + size = 196 and
  t = size/2 + 2*size = 327, as argued above.

=> (@jmd-dk) One way of doing this in practice (obtaining A(t) without actually
ever finding a, b and c) is through the Lagrange interpolation polynomial. The
final answer is then A(26501365). Note that our formula A(t) is only valid for t
of the form t = size/2 + n*size = 65 + n*131, which is exactly the form of the
requested step number 26501365 = 65 + 202300*131).

3957 = a 65^2  + b 65 + c
7791 = a 196^2 + b 196 + c
7787 = a 327^2 + b 327 + c
*/

// <x, f(x)>
uint64_t interpolate(vector<pair<int, int>> &poly, uint64_t xi) {
  uint64_t result = 0; // Initialize result

  for (int i = 0; i < poly.size(); i++) {
    // Compute individual terms of above formula
    uint64_t term = poly[i].second;
    for (int j = 0; j < poly.size(); j++) {
      if (j != i)
        term = term * (xi - poly[j].first) /
               uint64_t(poly[i].first - poly[j].first);
    }

    // Add current term to result
    result += term;
  }

  return result;
}

uint64_t xyhash(int x, int y) { return (uint64_t)x + ((uint64_t)y << 32); }

void solve(Graph &graph, Pos &start, int step) {
  queue<Pos> bfsq;
  bfsq.push(start);
  assert(graph[start.first][start.second] == 'S');
  vector<pair<int, uint64_t>> polys;
  for (int j = 0; j <= step; ++j) {
    int len = (int)bfsq.size();
    unordered_set<uint64_t> visited;
    int param = 0;
    for (int i = 0; i < len; ++i) {
      Pos &p = bfsq.front();
      bfsq.pop();
      int x = p.second, y = p.first;
      if (visited.count(xyhash(x, y)))
        continue;
      visited.insert(xyhash(x, y));
      ++param;
      if (x > 0 && graph[y][x - 1] != '#')
        bfsq.emplace(y, x - 1);
      if (x < (int)graph[0].length() - 1 && graph[y][x + 1] != '#')
        bfsq.emplace(y, x + 1);
      if (y > 0 && graph[y - 1][x] != '#')
        bfsq.emplace(y - 1, x);
      if (y < (int)graph.size() - 1 && graph[y + 1][x] != '#')
        bfsq.emplace(y + 1, x);
    }
    if (j == 65 || j == 196 || j == 327) {
      polys.emplace_back(j, param);
      cout << j << " " << param << "\n";
    }
    if (polys.size() == 3)
      break;
  }

  // ref:
  // https://github.com/TSoli/advent-of-code/blob/7d6d8961a1bc955e479347c44d70012742c19053/2023/day21b/solution.cpp#L127-L134
  uint64_t p1{polys[0].second};
  uint64_t p2{polys[1].second - polys[0].second};
  uint64_t p3{polys[2].second - polys[1].second};
  uint64_t ip{step / static_cast<uint64_t>(131)};

  // solve the quadratic
  uint64_t totalSteps{p1 + p2 * ip + (ip * (ip - 1) / 2) * (p3 - p2)};
  std::cout << "ans = " << totalSteps << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  Graph graph;
  int expandnum = 10;
  for (std::string line; getline(input, line);) {
    string expanded;
    for (int i = 0; i < expandnum; ++i)
      expanded += line;
    // graph.push_back(line);
    graph.push_back(expanded);
  }
  Graph newGraph;
  for (int i = 0; i < expandnum; ++i) {
    for (int j = 0; j < graph.size(); ++j)
      newGraph.push_back(graph[j]);
  }

  Pos start;
  for (int i = 131 * expandnum / 2; i < (int)newGraph.size(); ++i) {
    for (int j = 131 * expandnum / 2; j < (int)newGraph[0].length(); ++j)
      if (newGraph[i][j] == 'S') {
        start = {i, j};
        break;
      }
    if (start.first)
      break;
  }

  start = {131 / 2 + 131 * expandnum / 2, 131 * expandnum / 2 - 131 / 2 - 1};
  // cout << newGraph[start.first] << " " << start.first << "\n";

  // cout << newGraph[start.first][start.second] << "\n";
  //  return 0;

  // solve(graph, start, 327 + 1);
  solve(newGraph, start, 26501365);
  return 0;
}
