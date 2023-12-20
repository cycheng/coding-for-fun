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
bool isSmudge(const string &s0, const string &s1) {
  bool hasOneDiff = false;
  for (uint32_t i = 0; i < s0.length(); ++i)
    if (s0[i] != s1[i]) {
      if (hasOneDiff) {
        hasOneDiff = false;
        break;
      }
      hasOneDiff = true;
    }
  return hasOneDiff;
}

uint32_t scan_horizontal(const Graph &graph) {
  uint32_t numRow = graph.size();

  vector<uint32_t> matchedSmudgeRows;
  for (uint32_t i = 0; i < numRow - 1; ++i) {
    if (graph[i] == graph[i + 1] || isSmudge(graph[i], graph[i + 1]))
      matchedSmudgeRows.push_back(i);
  }

  uint32_t maxDist = 0, distToRow0 = 0;
  for (uint32_t start : matchedSmudgeRows) {
    int up = start, down = start + 1;
    bool fixedOneSmudge = isSmudge(graph[up], graph[down]);
    --up, ++down;
    while (0 <= up && (uint32_t)down < numRow) {
      if (graph[up] != graph[down]) {
        if (fixedOneSmudge)
          break;
        if (isSmudge(graph[up], graph[down]))
          fixedOneSmudge = true;
        else
          break;
      }
      --up, ++down;
    }
    if ((up + 1 == 0 || (uint32_t)down == numRow) &&
        maxDist < (uint32_t)(down - up) && fixedOneSmudge) {
      maxDist = down - up;
      distToRow0 = start + 1;
    }
  }
  cout << "maxReflection " << distToRow0 << "\n";
  return distToRow0;
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
  uint32_t h = scan_horizontal(graph);
  uint32_t v = scan_horizontal(transpose(graph));

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
