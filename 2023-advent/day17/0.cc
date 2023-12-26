// https://adventofcode.com/2023/day/16

#include <bits/stdc++.h>
using namespace std;

/* format
2413432311323
3215453535623
3255245654254
3446585845452
4546657867536
1438598798454
4457876987766
3637877979653
4654967986887
4564679986453
1224686865563
2546548887735
4322674655533
*/

using IntGraph = vector<vector<int>>;
// using DirGraph = vector<vector<uint8_t>>;
using CharGraph = vector<string>;

enum {
  north = 0,
  west,
  south,
  east,
};

// The starting point: top-left, dest: bottom-right.
//
// It can move at most three blocks in a single direction
//
// After entering each city block, it may only turn left, continue straight, or
// turn right.

int MinEng = 0x7fffffff;
CharGraph CGraph;

uint8_t turnLeft(uint8_t dir) {
  switch (dir) {
  case north: return west;
  case west: return south;
  case south: return east;
  case east: return north;
  }
  assert(false);
  return 0;
}

uint8_t turnRight(uint8_t dir) {
  switch (dir) {
  case north: return east;
  case east: return south;
  case south: return west;
  case west: return north;
  }
  assert(false);
  return 0;
}

pair<int, int> move(uint8_t dir, pair<int, int> pos) {
  int row = pos.first;
  int col = pos.second;

  switch (dir) {
  case north: return {row - 1, col};
  case south: return {row + 1, col};
  case west: return {row, col - 1};
  case east: return {row, col + 1};
  }
  assert(false);
  return {0, 0};
}

char dirText(uint8_t dir) {
  switch (dir) {
  case north: return '^';
  case west: return '<';
  case south: return 'v';
  case east: return '>';
  }
  assert(false);
  return '?';
}

struct Block {
  pair<int, int> pos;
  uint8_t dir = 0;
  uint8_t step = 0;
  int totalEng = 0; // parents, exclude myslef

  Block() = default;
  Block(pair<int, int> pos, uint8_t dir, uint8_t step, int totalEng)
      : pos(pos), dir(dir), step(step), totalEng(totalEng) {}
};

auto BlockCmp = [](Block a, Block b) { return a.totalEng > b.totalEng; };

// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
void dijkstra_shortest_path(const IntGraph &graph) {
  priority_queue<Block, vector<Block>, decltype(BlockCmp)> pq(BlockCmp);

  // Because you already start in the top-left block, you don't incur that
  // block's heat loss unless you leave that block and then return to it.
  pq.emplace(pair<int, int>(0, 0), east, 3, -graph[0][0]);
  pq.emplace(pair<int, int>(0, 0), south, 3, -graph[0][0]);

  // 4 directions with 3 steps
  // 0..2: north, 3..5: west, ..
  IntGraph curEngGraph[4 * 3];
  for (int i = 0; i < 4 * 3; ++i)
    curEngGraph[i].resize(graph.size(),
                          vector<int>(graph[0].size(), 0x7fffffff));

  while (pq.size()) {
    Block block = pq.top();
    pq.pop();

    int row = block.pos.first;
    int col = block.pos.second;
    if (row < 0 || col < 0 || row == (int)graph.size() ||
        col == (int)graph[0].size() || block.step == 0) {
      continue;
    }

    int curTotalEng = block.totalEng + graph[row][col];
    if (row == (int)graph.size() - 1 && col == (int)graph[0].size() - 1) {
      MinEng = min(MinEng, curTotalEng);
      cout << "MinEng " << MinEng << "\n";
      continue;
    }

    if (curTotalEng >= MinEng)
      continue;

    uint8_t dir = block.dir;

    if (curEngGraph[dir * 3 + block.step - 1][row][col] <= curTotalEng)
      continue;

    curEngGraph[dir * 3 + block.step - 1][row][col] = curTotalEng;

    Block newBlock;
    newBlock.totalEng = curTotalEng;
    newBlock.step = 3;

    newBlock.pos = move(turnLeft(dir), block.pos);
    newBlock.dir = turnLeft(dir);
    pq.push(newBlock);

    newBlock.pos = move(turnRight(dir), block.pos);
    newBlock.dir = turnRight(dir);
    pq.push(newBlock);

    newBlock.pos = move(dir, block.pos);
    newBlock.dir = dir;
    newBlock.step = block.step - 1;
    pq.push(newBlock);
  }

#if 0
  cout << "\n";
  for (auto &row : CGraph) {
    for (char c : row)
      cout << c;
    cout << "\n";
  }
#endif

#if 0
  IntGraph minEngGraph;
  minEngGraph.resize(graph.size(), vector<int>(graph[0].size(), 0x7fffffff));

  int numRow = graph.size(), numCol = graph[0].size();
  for (int i = 0; i < 4 * 3; ++i) {
    for (int j = 0; j < numRow; ++j)
      for (int k = 0; k < numCol; ++k)
        minEngGraph[j][k] = min(minEngGraph[j][k], curEngGraph[i][j][k]);
  }

  cout << "\n";
  for (auto &row : graph) {
    for (auto c : row)
      cout << setw(5) << (int)c;
    cout << "\n";
  }

  cout << "\n";
  for (auto &row : minEngGraph) {
    for (auto v : row)
      cout << setw(5) << v;
    cout << "\n";
  }
  cout << "\n";
#endif
}

uint32_t solve(IntGraph &graph) {
  dijkstra_shortest_path(graph);

  return MinEng;
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  IntGraph graph;
  for (std::string line; getline(input, line);) {
    graph.push_back(vector<int>(line.length(), 0));
    for (int i = 0; i < (int)line.length(); ++i)
      graph.back()[i] = line[i] - '0';

    CGraph.push_back(line);
  }

  cout << "ans = " << solve(graph) << "\n";

  return 0;
}
