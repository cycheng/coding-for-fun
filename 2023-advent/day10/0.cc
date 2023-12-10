// https://adventofcode.com/2023/day/7

#include <bits/stdc++.h>
using namespace std;

/* format
..F7.
.FJ|.
SJ.L7
|F--J
LJ...
*/

vector<string> Graph;
vector<vector<bool>> Visited;
vector<vector<uint32_t>> NumGraph;
uint32_t Invalid = 0xffffffff;
struct Pos {
  uint32_t row = Invalid, column = 0;
  Pos() = default;
  Pos(uint32_t r, uint32_t c) : row(r), column(c) {}
  bool valid() { return row != Invalid; }
  bool operator==(const Pos &p) const {
    return p.row == row && p.column == column;
  }
};
Pos StartPos;

bool canUp(char c) {
  switch (c) {
  case '|':
  case '7':
  case 'F': return true;
  default: return false;
  }
}
bool canDown(char c) {
  switch (c) {
  case '|':
  case 'L':
  case 'J': return true;
  default: return false;
  }
}
bool canLeft(char c) {
  switch (c) {
  case '-':
  case 'L':
  case 'F': return true;
  default: return false;
  }
}
bool canRight(char c) {
  switch (c) {
  case '-':
  case 'J':
  case '7': return true;
  default: return false;
  }
}

// Pos <row, column>
vector<Pos> nextPos(const Pos &curPos) {
  uint32_t r = curPos.row, c = curPos.column;
  char symbol = Graph[r][c];
  vector<Pos> next;

  switch (symbol) {
  case '|': {
    if (r)
      next.emplace_back(r - 1, c);
    if (r + 1 < Graph.size())
      next.emplace_back(r + 1, c);
    break;
  }
  case '-': {
    if (c)
      next.emplace_back(r, c - 1);
    if (c + 1 < Graph[r].length())
      next.emplace_back(r, c + 1);
    break;
  }
  case 'L': {
    if (r)
      next.emplace_back(r - 1, c);
    if (c + 1 < Graph[r].length())
      next.emplace_back(r, c + 1);
    break;
  }
  case 'J': {
    if (r)
      next.emplace_back(r - 1, c);
    if (c)
      next.emplace_back(r, c - 1);
    break;
  }
  case '7': {
    if (r + 1 < Graph.size())
      next.emplace_back(r + 1, c);
    if (c)
      next.emplace_back(r, c - 1);
    break;
  }
  case 'F': {
    if (r + 1 < Graph.size())
      next.emplace_back(r + 1, c);
    if (c + 1 < Graph[r].length())
      next.emplace_back(r, c + 1);
    break;
  }
  case '.': {
    break;
  }
  case 'S': {
    Visited[r][c] = true;
    if (r && canUp(Graph[r - 1][c]))
      next.emplace_back(r - 1, c);

    if (r + 1 < Graph.size() && canDown(Graph[r + 1][c]))
      next.emplace_back(r + 1, c);

    if (c && canLeft(Graph[r][c - 1]))
      next.emplace_back(r, c - 1);

    if (c + 1 < Graph[r].length() && canRight(Graph[r][c + 1]))
      next.emplace_back(r, c + 1);
    break;
  }
  default: assert(false && "unknow char");
  }

  for (size_t i = next.size(); i > 0; --i) {
    auto pos = next[i - 1];
    if (Graph[pos.row][pos.column] == '.') {
      next.erase(next.begin() + (i - 1));
      continue;
    }
  }
  return next;
}

uint32_t Maxstep = 0;
void dfs(const Pos &start0, const Pos &start1, uint32_t step) {
  for (auto &next0 : nextPos(start0)) {
    if (Visited[next0.row][next0.column])
      continue;
    for (auto &next1 : nextPos(start1)) {
      // if (NumGraph[next0.row][next0.column] < step + 1)
      if (Visited[next1.row][next1.column])
        continue;

      if (next0 == next1) {
        Maxstep = max(Maxstep, step + 1);
      } else {
        Visited[next0.row][next0.column] = true;
        Visited[next1.row][next1.column] = true;

        NumGraph[next0.row][next0.column] = step + 1;
        NumGraph[next1.row][next1.column] = step + 1;
        dfs(next0, next1, step + 1);
        Visited[next0.row][next0.column] = false;
        Visited[next1.row][next1.column] = false;
      }
    }
  }
}

void solve() {
  auto nexts = nextPos(StartPos);
  assert(nexts.size() == 2);
  dfs(nexts[0], nexts[1], 1);
#if 0
  uint32_t step = 0;
  queue<Pos> bfsq;
  bfsq.push(StartPos);
  while (!bfsq.empty()) {
    size_t len = bfsq.size();
    step++;
    for (size_t i = 0; i < len; ++i) {
      for (auto &next : nextPos(bfsq.front())) {
        NumGraph[next.row][next.column] = step;
        bfsq.push(next);
      }
      bfsq.pop();
    }
  }
#endif
  uint32_t ans = 0;
  for (auto &row : NumGraph) {
    for (uint32_t v : row) {
      ans = max(v, ans);
      if (v)
        cout << setw(3) << v;
      else
        cout << "  .";
    }
    cout << "\n";
  }

  cout << "ans = " << Maxstep << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    Graph.push_back(line);
    NumGraph.push_back(vector<uint32_t>(line.size(), 0));
    Visited.push_back(vector<bool>(line.length(), false));
    if (!StartPos.valid())
      for (size_t i = 0; i < line.size(); ++i)
        if (line[i] == 'S')
          StartPos = Pos(Graph.size() - 1, i);
  }
  solve();
  return 0;
}
