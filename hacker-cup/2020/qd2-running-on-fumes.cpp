// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/D2
// #38, #like, #segment tree

#include <bits/stdc++.h>
using namespace std;

// Reference:
// [1] https://youtu.be/XTXB46IIeHM?t=20
// [2] William Lin (tmwilliamlin168)'s contest solution
// [3] https://www.geeksforgeeks.org/segment-tree-efficient-implementation/

// See [3]
//
// For example: n = 5

constexpr int64_t INF = 1e15; // numeric_limits<int64_t>::max();

class segment_tree {
  size_t n;
  vector<int64_t> array;

public:
  segment_tree(size_t n) : n(n) { array.resize(n * 2, INF); }

  void update(size_t pos, int64_t val) {
    assert(pos < n && "out of bound update.");
    // update from leaf node to root.
    for (pos += n; pos > 0; pos /= 2)
      array[pos] = min(array[pos], val);
  }

  // query minimum value in range [left, right]
  int64_t query(size_t left, size_t right) const {
    int64_t res = INF;
    for (left += n, right += n + 1; left < right; left /= 2, right /= 2) {
      if (left & 1)
        res = min(res, array[left++]);
      if (right & 1)
        res = min(res, array[--right]);
    }
    return res;
  }
};

struct node {
  // "from": the node which visit this node
  int from = 0;
  // dist from root to this node
  int dist = 0;
  node() = default;
};

vector<node> bfs_build_path(const vector<vector<int>> &graph, int from) {
  const uint32_t n = graph.size();
  vector<bool> visited(n);
  vector<node> path(n);
  queue<int> worklist;
  worklist.push(from);
  while (!worklist.empty()) {
    uint32_t size = worklist.size();
    for (uint32_t i = 0; i < size; ++i) {
      int u = worklist.front();
      visited[u] = true;
      worklist.pop();
      for (int v : graph[u])
        // if (v != path[u].from) {
        if (!visited[v]) {
          worklist.push(v);
          path[v].from = u;
          path[v].dist = path[u].dist + 1;
        }
    }
  }
  return path;
}

int64_t bfs_backward_find_min_cost(const vector<vector<int>> &graph,
                                   const vector<int> &cost,
                                   const vector<node> &path, int fuel,
                                   int start, int end) {
  const uint32_t n = path.size();
  vector<bool> visited(n);
  segment_tree sgt(n + fuel);
  sgt.update(path[end].dist, 0);
  for (int main = path[end].from; main != 0; main = path[main].from) {
    // search in subtree for a main node.
    queue<int> worklist;
    worklist.push(main);
    while (!worklist.empty()) {
      int u = worklist.front();
      visited[u] = true;
      worklist.pop();

      if (path[u].dist - path[main].dist > fuel || cost[u] == 0)
        continue;

      int64_t mincost = sgt.query(
          path[main].dist,
          min(path[main].dist + fuel + path[u].dist - path[main].dist, (int)n));
      sgt.update(path[u].dist, mincost);

      for (int v : graph[u])
        if (!visited[v])
          worklist.push(v);
    }
  }
  return sgt.query(start, start);
}

int64_t solve() {
  int n, m, a, b;
  cin >> n >> m >> a >> b;

  // build the tree graph.
  // 0 is null.
  vector<vector<int>> graph(n);
  vector<int> costs(n);
  for (int i = 0; i < n; ++i) {
    int parent;
    cin >> parent >> costs[i];
    if (parent == 0)
      continue;
    --parent;
    graph[parent].push_back(i);
    graph[i].push_back(parent);
  }

  // build path start from a to all other nodes
  vector<node> path = bfs_build_path(graph, a);

  return bfs_backward_find_min_cost(graph, costs, path, m, a, b);
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cout << "Case #" << i << ": " << solve();
    cout << endl;
  }
  return 0;
}