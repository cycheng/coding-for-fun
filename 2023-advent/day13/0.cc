// https://adventofcode.com/2023/day/13

#include <bits/stdc++.h>
using namespace std;

/* format

#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

*/

using Graph = vector<string>;
using IntGraph = vector<int>;

#if 1
uint32_t scan_horizontal(const Graph &graph) {
  uint32_t numRow = graph.size();

  vector<uint32_t> matchedRows;
  for (uint32_t i = 0; i < numRow - 1; ++i) {
    if (graph[i] == graph[i + 1])
      matchedRows.push_back(i);
  }

  uint32_t maxDist = 1, distToRow0 = 0;
  for (uint32_t start : matchedRows) {
    int up = start, down = start + 1;
    while (0 <= up && (uint32_t)down < numRow) {
      if (graph[up] != graph[down])
        break;
      --up, ++down;
    }
    if ((up + 1 == 0 || down == numRow) && maxDist < (uint32_t)(down - up)) {
      maxDist = down - up;
      distToRow0 = start + 1;
    }
  }
  // cout << "maxReflection " << distToRow0 << "\n";
  return distToRow0;
}

// ref:
// https://github.com/TSoli/advent-of-code/blob/main/2023/day13a/solution.cpp
uint32_t scan_horizontal_tsoil(const Graph &graph) {
  int maxReflection{0};
  for (int i = 0; i < graph.size() - 1; ++i) {
    int j{0};
    int currReflection{0};
    while (i - j >= 0 && i + j + 1 < graph.size() &&
           graph[i - j] == graph[i + j + 1]) {
      ++currReflection;
      ++j;
    }

    if (i - j + 1 == 0 || i + j + 1 == graph.size()) {
      maxReflection = std::max(maxReflection, currReflection + i - j + 1);
    }
  }
  // cout << "maxReflection " << maxReflection << "\n";
  return maxReflection;
}

#else

IntGraph genIntGraph(const Graph &graph) {
  IntGraph igraph;
  int idx = 0;
  unordered_map<string, int> m2i;
  for (auto &str : graph) {
    auto iter = m2i.find(str);
    if (iter == m2i.end()) {
      igraph.push_back(idx);
      m2i[str] = idx++;
    } else {
      igraph.push_back(iter->second);
    }
  }
  return igraph;
}

uint32_t scan_horizontal(const Graph &graph) {

  IntGraph igraph = genIntGraph(graph);

  for (int v : igraph)
    cout << v << " ";
  cout << "\n";
  uint32_t numRow = graph.size();
  uint32_t middle = 1;
  for (; middle < numRow; ++middle)
    if (igraph[middle] - igraph[middle - 1] != 1)
      break;

  if (middle == numRow)
    return 0;

  if (abs(igraph[middle] - igraph[middle - 1]) != 1 &&
      igraph[middle] != igraph[middle - 1])
    return 0;

  if (abs((int)(numRow - middle) - (int)middle) > 1)
    return 0;

  for (uint32_t right = middle; right < numRow - 1; ++right) {
    if (igraph[right] - igraph[right + 1] != 1)
      return 0;
  }

  cout << "middle " << middle << "\n";
  return middle;
}
#endif

void printGraph(Graph &graph) {
  for (auto &str : graph) {
    cout << str << "\n";
  }
}

Graph transpose(Graph &graph) {
  uint32_t numRow = graph.size(), numCol = graph[0].length();
  Graph tgraph(numCol);
  for (uint32_t i = 0; i < numRow; ++i)
    for (uint32_t j = 0; j < numCol; ++j)
      tgraph[j].push_back(graph[i][j]);
  return tgraph;
}

uint32_t solve(Graph &graph) {
  uint32_t h = scan_horizontal_tsoil(graph);
  uint32_t v = scan_horizontal_tsoil(transpose(graph));

  uint32_t cyh = scan_horizontal(graph);
  uint32_t cyv = scan_horizontal(transpose(graph));

  if (cyh != h || cyv != v) {
    printGraph(graph);
    cout << "h v " << h << " " << v << ", cyh cyv " << cyh << " " << cyv
         << "\n";
    cout << "\n\n";
  }

  if (h > v)
    return 100 * h;
  return v;
}

constexpr int NumExpansion = (5 - 1);
// constexpr int NumExpansion = (1 - 1);

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  Graph graph;
  for (std::string line; getline(input, line);) {
    if (line.empty()) {
      sum += solve(graph);
      graph.clear();
    } else {
      graph.emplace_back(line);
    }
  }
  sum += solve(graph);
  cout << "ans = " << sum << "\n";

  return 0;
}
