// https://www.facebook.com/codingcompetitions/hacker-cup/2019/qualification-round/problems/D
//
// #like

#include <bits/stdc++.h>
using namespace std;

// struct node {
//   int num_in = 0;
//   vector<int> ancestor;
//   vector<int> descendant;
// }

bool hasCycle(const vector<vector<int>> &adj, vector<bool> &visited, int u,
              int parent) {
  visited[u] = true;
  for (const int &v : adj[u]) {
    if (visited[v] && v != parent) {
      // cout << "find cycle v = " << v << " p = " << parent << endl;
      return true;
    }
    if (!visited[v])
      if (hasCycle(adj, visited, v, u))
        return true;
  }
  return false;
}

bool can_reach(const vector<int> &parent, int x, int y) {
  while (x && x != y) {
    if (x != y)
      x = parent[x];
  }
  return x == y;
}

bool is_valid_lca(const vector<int> &parent, int x, int y, int lca) {
  // cout << "check " << x << " parent[x] " << parent[x] << " " << y
  //      << " parent[y] " << parent[y] << " " << lca << endl;
  if (x == 0 || y == 0)
    return false;

  if (parent[x] == lca && parent[x] == lca)
    return true;

  if (parent[x] == lca)
    return is_valid_lca(parent, x, parent[y], lca);

  if (parent[y] == lca)
    return is_valid_lca(parent, parent[x], y, lca);

  return is_valid_lca(parent, parent[x], parent[y], lca);
}

void solve() {
  int n, m;
  cin >> n >> m;

  vector<vector<int>> adj(n + 1);
  vector<tuple<int, int, int>> edges;
  vector<int> vert_in_degree(n + 1);

  // build adjacent graph
  for (int i = 0; i < m; ++i) {
    int x, y, lca;
    cin >> x >> y >> lca;
    if (x != lca) {
      adj[lca].push_back(x);
      adj[x].push_back(lca);
      ++vert_in_degree[x];
    }

    if (y != lca) {
      adj[lca].push_back(y);
      adj[y].push_back(lca);
      ++vert_in_degree[y];
    }
    edges.emplace_back(x, y, lca);
  }

  // build tree in topological order
  // vector<vector<int>> tree(n + 1);
  vector<int> parent(n + 1);
  // <parent, u>
  vector<pair<int, int>> worklist;
  worklist.reserve(n);

  int root = 0;
  for (int i = 1; i <= n; ++i)
    if (vert_in_degree[i] == 0) {
      root = i;
      break;
    }

  for (int i = root + 1; i <= n; ++i)
    if (vert_in_degree[i] == 0) {
      adj[root].push_back(i);
      adj[i].push_back(root);
      ++vert_in_degree[i];
    }

  worklist.emplace_back(0, root);

  for (int i = 0; i < (int)worklist.size(); ++i) {
    int p = worklist[i].first;
    int u = worklist[i].second;
    parent[u] = p;

    // if (p != u) {
    //   tree[u].push_back(p);
    //   tree[p].push_back(u);
    // }

    for (int v : adj[u])
      if (vert_in_degree[v])
        if (--vert_in_degree[v] == 0)
          worklist.emplace_back(u, v);
  }

  // parent[root] = 0;

  // run dfs to detect cycle.
  // vector<bool> visited(n + 1);
  // for (int u = 1; u <= n; ++u)
  //   if (!visited[u])
  //     if (hasCycle(tree, visited, u, 0)) {
  //       cout << " Impossible" << endl;
  //       return;
  //     }
  // for (int p : parent)
  //   cout << " " << p;
  // cout << endl;
  for (const auto e : edges) {
    int x, y, lca;
    tie(x, y, lca) = e;
    // cout << "weird" << endl;
    bool status = true;
    if (x == lca)
      status = can_reach(parent, y, lca);
    else if (y == lca)
      status = can_reach(parent, x, lca);
    else {
      status = !can_reach(parent, x, y);
      status &= !can_reach(parent, y, x);
      status &= is_valid_lca(parent, x, y, lca);
    }
    if (!status) {
      cout << " Impossible";
      return;
    }
  }

  for (int i = 1; i <= n; ++i)
    cout << " " << parent[i];
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cout << "Case #" << i << ":";
    solve();
    cout << endl;
  }
  return 0;
}