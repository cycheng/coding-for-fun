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
  int totalEng = 0; // include myslef

  Block() = default;
  Block(pair<int, int> pos, uint8_t dir, uint8_t step, int totalEng)
      : pos(pos), dir(dir), step(step), totalEng(totalEng) {}

  // don't compare totalEng.
  bool operator==(const Block &b) const {
    return pos == b.pos && dir == b.dir && step == b.step;
  }
};

// https://www.geeksforgeeks.org/how-to-create-an-unordered_set-of-user-defined-class-or-struct-in-c/
struct BlockHash {
  size_t operator()(const Block &b) const {
    size_t value =
        b.pos.first | (b.pos.second << 8) | (b.step << 16) | (b.dir << 20);
    return value;
  }
};

auto BlockCmp = [](Block a, Block b) { return a.totalEng > b.totalEng; };

// https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-using-priority_queue-stl/
uint32_t dijkstra_shortest_path(const IntGraph &graph) {
  priority_queue<Block, vector<Block>, decltype(BlockCmp)> pq(BlockCmp);

  unordered_set<Block, BlockHash> visited;

  // Because you already start in the top-left block, you don't incur that
  // block's heat loss unless you leave that block and then return to it.
  pq.emplace(pair<int, int>(0, 0), east, 0, -graph[0][0]);
  pq.emplace(pair<int, int>(0, 0), south, 0, -graph[0][0]);
  int MinEng = 0x7fffffff;

  while (pq.size()) {
    Block block = pq.top();
    pq.pop();

    int row = block.pos.first;
    int col = block.pos.second;
    if (row < 0 || col < 0 || row == (int)graph.size() ||
        col == (int)graph[0].size()) {
      continue;
    }

    const int curTotalEng = block.totalEng + graph[row][col];
    const uint8_t step = block.step;
    if (row == (int)graph.size() - 1 && col == (int)graph[0].size() - 1 &&
        step > 3) {
      MinEng = min(MinEng, curTotalEng);
      cout << "MinEng " << MinEng << "\n";
      continue;
    }

    if (curTotalEng >= MinEng)
      continue;

    const uint8_t dir = block.dir;
    auto iter = visited.find(block);
    if (iter != visited.end() && iter->totalEng <= curTotalEng) {
      continue;
    }

    if (iter != visited.end())
      visited.erase(iter);
    visited.insert(block);

    Block newBlock;
    newBlock.totalEng = curTotalEng;

    if (step < 10) {
      newBlock.pos = move(dir, block.pos);
      newBlock.dir = dir;
      newBlock.step = step + 1;
      pq.push(newBlock);
    }

    if (step > 3) {
      newBlock.step = 1;

      newBlock.pos = move(turnLeft(dir), block.pos);
      newBlock.dir = turnLeft(dir);
      pq.push(newBlock);

      newBlock.pos = move(turnRight(dir), block.pos);
      newBlock.dir = turnRight(dir);
      pq.push(newBlock);
    }
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
  return MinEng;
}

uint32_t solve(IntGraph &graph) { return dijkstra_shortest_path(graph); }

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  IntGraph graph;
  for (std::string line; getline(input, line);) {
    graph.push_back(vector<int>(line.length(), 0));
    for (int i = 0; i < (int)line.length(); ++i)
      graph.back()[i] = line[i] - '0';
  }

  cout << "ans = " << solve(graph) << "\n";

  return 0;
}
