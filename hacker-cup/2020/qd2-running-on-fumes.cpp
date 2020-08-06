// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/D2
// #38, #like, #dp, #segment tree

#include <bits/stdc++.h>
using namespace std;

#define DUMP(x)

// Reference:
// [1] https://youtu.be/XTXB46IIeHM?t=20
// [2] Chen-Hao Chang's (cchao) contest solution.
// [3] William Lin (tmwilliamlin168)'s contest solution
// [4] https://www.geeksforgeeks.org/segment-tree-efficient-implementation/

// See [4]
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
    pos += n;
    // update from leaf node to root.
    if (array[pos] <= val)
      return;
    array[pos] = val;
    for (; pos > 1; pos /= 2)
      array[pos / 2] = min(array[pos], array[pos ^ 1]);
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

  void dump() const {
    for (uint32_t i = 0; i < array.size(); ++i) {
      if (array[i] >= INF)
        cout << "x ";
      else
        cout << array[i] << " ";
      if (i % 12 == 11)
        cout << endl;
    }
    cout << endl;
  }
};

struct node {
  // "from": the node which visit this node
  int from = 0;
  // dist from root to this node
  int dist = 0;
  node() = default;
};

void dump_path(const vector<node> &path) {
  for (const auto &p : path)
    cout << "(" << p.from << ", " << p.dist << "), ";
  cout << endl;
}

vector<node> bfs_build_path(const vector<vector<int>> &graph, int from) {
  const uint32_t n = graph.size();
  vector<node> path(n);
  queue<int> worklist;
  path[from].from = -1;
  worklist.push(from);
  while (!worklist.empty()) {
    uint32_t size = worklist.size();
    for (uint32_t i = 0; i < size; ++i) {
      int u = worklist.front();
      worklist.pop();
      for (int v : graph[u])
        if (v != path[u].from) {
          worklist.push(v);
          path[v].from = u;
          path[v].dist = path[u].dist + 1;
        }
    }
  }
  return path;
}

// ref: Chen-Hao Chang's (cchao) contest solution.
int64_t bfs_backward_find_min_cost(const vector<vector<int>> &graph,
                                   const vector<int> &cost,
                                   const vector<node> &path, int fuel,
                                   int start, int end) {
  const uint32_t n = graph.size();

  vector<bool> visited(n);
  segment_tree sgt(n);

  sgt.update(path[end].dist, 0);
    DUMP(cout << "process " << main << endl);

  for (int main = path[end].from; main != -1; main = path[main].from) {
    // search in subtree for a main node.
    queue<int> worklist;
    worklist.push(main);
    while (!worklist.empty()) {
      int u = worklist.front();
      visited[u] = true;
      worklist.pop();

      const int main_dist = path[main].dist, u_dist = path[u].dist;
      if (u_dist - main_dist > fuel)
        continue;

      if (cost[u]) {
        int back_steps = min(main_dist + fuel + main_dist - u_dist, (int)n);
        int64_t mincost = sgt.query(main_dist, back_steps);
        sgt.update(u_dist, mincost + cost[u]);

        DUMP(cout << "  query: node " << u << "(dist =" << u_dist << "), from "
                  << main_dist << " to dist = " << back_steps << " = "
                  << mincost << endl);
      }

      for (int v : graph[u])
        if (!visited[v] && path[v].from == u)
          worklist.push(v);
    }
  }
  int64_t ans = INF; // sgt.query(start, start);

  if ((int)n == fuel)
    --fuel;
  for (int i = 1; i <= fuel; ++i) {
    ans = min(ans, sgt.query(i, i));
  }
  DUMP(sgt.dump());

  return ans >= INF ? -1 : ans;
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

  --a, --b;
  // build path start from a to all other nodes
  vector<node> path = bfs_build_path(graph, a);
  DUMP(dump_path(path));

  costs[b] = 0;
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