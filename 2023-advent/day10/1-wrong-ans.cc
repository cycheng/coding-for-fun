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
vector<vector<bool>> MaxLoop;
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
        if (Maxstep < step + 1) {
          Visited[next0.row][next0.column] = true;
          NumGraph[next0.row][next0.column] = step + 1;
          Maxstep = step + 1;
          MaxLoop = Visited;
        }
      } else {
        Visited[next0.row][next0.column] = true;
        Visited[next1.row][next1.column] = true;

        if (NumGraph[next0.row][next0.column] < step + 1)
          NumGraph[next0.row][next0.column] = step + 1;

        if (NumGraph[next1.row][next1.column] < step + 1)
          NumGraph[next1.row][next1.column] = step + 1;
        dfs(next0, next1, step + 1);
        Visited[next0.row][next0.column] = false;
        Visited[next1.row][next1.column] = false;
      }
    }
  }
}

void visitOuerRegion(uint32_t r, uint32_t c) {
  if (Visited[r][c])
    return;
  Visited[r][c] = true;
  vector<Pos> next;

  if (r)
    next.emplace_back(r - 1, c);

  if (r + 1 < Graph.size())
    next.emplace_back(r + 1, c);

  if (c)
    next.emplace_back(r, c - 1);

  if (c + 1 < Graph[r].length())
    next.emplace_back(r, c + 1);

  for (size_t i = next.size(); i > 0; --i) {
    auto pos = next[i - 1];
    if (Graph[pos.row][pos.column] != '.') {
      next.erase(next.begin() + (i - 1));
      continue;
    }
  }

  for (auto &pos : next)
    visitOuerRegion(pos.row, pos.column);
}

bool isInLoop(uint32_t r, uint32_t c, uint32_t numRow, uint32_t numCol) {
  uint32_t cnt = 0;
  // number of intersections in the horizontal?
  for (uint32_t h = 0; h < c; ++h)
    if (MaxLoop[r][h])
      ++cnt;
  if (cnt % 2)
    return true;

  for (uint32_t h = c + 1; h < numCol; ++h)
    if (MaxLoop[r][h])
      ++cnt;
  if (cnt % 2)
    return true;

  // number of intersections in the vertical?
  for (uint32_t v = 0; v < r; ++v)
    if (MaxLoop[v][c])
      ++cnt;
  if (cnt % 2)
    return true;

  for (uint32_t v = r + 1; v < numRow; ++v)
    if (MaxLoop[v][c])
      ++cnt;
  if (cnt % 2)
    return true;

  return false;
}

void solve() {
  auto nexts = nextPos(StartPos);
  assert(nexts.size() == 2);
  Visited[nexts[0].row][nexts[0].column] = true;
  Visited[nexts[1].row][nexts[1].column] = true;
  NumGraph[nexts[0].row][nexts[0].column] = 1;
  NumGraph[nexts[1].row][nexts[1].column] = 1;
  const uint32_t numRow = NumGraph.size(), numCol = NumGraph[0].size();

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
  NumGraph[StartPos.row][StartPos.column] = Invalid;
  for (auto &row : NumGraph) {
    for (uint32_t v : row) {
      if (v == Invalid)
        cout << setw(3) << 'S';
      else if (v)
        cout << setw(3) << v;
      else
        cout << "  .";
    }
    cout << "\n";
  }

  for (auto &row : MaxLoop) {
    for (bool v : row) {
      if (v)
        cout << '+';
      else
        cout << '.';
    }
    cout << "\n";
  }

  // reset visited.
  for (auto &row : Visited)
    for (size_t i = 0; i < numCol; ++i)
      row[i] = false;

  // remove outer regions.
  for (size_t i = 0; i < numRow; ++i) {
    visitOuerRegion(i, 0);
    visitOuerRegion(i, numCol - 1);
    visitOuerRegion(0, i);
    visitOuerRegion(numRow - 1, i);
  }

  uint32_t ans = 0;
  for (size_t i = 0; i < numRow; ++i)
    for (size_t j = 0; j < numCol; ++j)
      if (!Visited[i][j] && Graph[i][j] == '.')
        if (isInLoop(i, j, numRow, numCol)) {
          ++ans;
          cout << i << " " << j << "\n";
        }

  cout << "ans = " << ans << "\n";
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
