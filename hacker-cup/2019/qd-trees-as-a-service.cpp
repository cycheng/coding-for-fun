// https://www.facebook.com/codingcompetitions/hacker-cup/2019/qualification-round/problems/D
//
// #like, #disjoint set, #45

#include <bits/stdc++.h>
using namespace std;

// disjoint_set from @mzchen
// see:
// https://leetcode.com/problems/similar-string-groups/discuss/132374/Short-C%2B%2B-solution-at-220ms-using-disjoint-set
class disjoint_set {
  vector<int> v;
  int sz;

public:
  disjoint_set(int n) { makeset(n); }
  disjoint_set() : sz(0) {}

  void makeset(int n) {
    v.resize(n);
    iota(v.begin(), v.end(), 0);
    sz = n;
  }

  int find(int i) {
    if (i != v[i])
      v[i] = find(v[i]);
    return v[i];
  }

  void join(int i, int j) {
    int si = find(i), sj = find(j);
    if (si != sj) {
      v[si] = v[sj];
      --sz;
    }
  }

  void reset(int i, int s) { v[i] = s; }

  int size() const { return sz; }
};

// ref:
// [1]
// https://www.facebook.com/notes/facebook-hacker-cup/hacker-cup-2019-qualification-round-solutions/2797355073613709/
// [2] Chen-Hao Chang's contest solution.
vector<vector<int>> descendents;
vector<tuple<int, int, int>> edges;
vector<int> in_degrees;
disjoint_set subtree_set;

bool failed;
bool build_tree(const vector<int> &nodes, const int parent, vector<int> &ans) {
  if (nodes.size() == 1) {
    ans[nodes[0]] = parent;
    return true;
  }

  unordered_set<int> candidates;
  candidates.insert(nodes.begin(), nodes.end());
  auto all_in_candidates = [&](int x, int y, int lca) {
    return candidates.count(x) && candidates.count(y) && candidates.count(lca);
  };

  // try to choose a root
  for (int root : nodes) {
    if (in_degrees[root] == 0) {
      // initialize set, each node is in unique set.
      for (int x : nodes)
        subtree_set.reset(x, x);

      // scan edges
      for (const auto &e : edges) {
        int x, y, lca;
        tie(x, y, lca) = e;

        // skip this edge since it has been checked.
        if (!all_in_candidates(x, y, lca))
          continue;

        // Since root != lca, lca, x, y must be in the same subtree.
        if (lca != root && x != root && y != root) {
          subtree_set.join(lca, x);
          subtree_set.join(lca, y);
        }
      }

      bool valid = true;
      for (const auto &e : edges) {
        int x, y, lca;
        tie(x, y, lca) = e;

        // skip this edge since it has been checked.
        if (!all_in_candidates(x, y, lca))
          continue;

        // lca is root, then the x and y should reside in different subtree.
        if (lca == root && x != root && y != root &&
            subtree_set.find(x) == subtree_set.find(y)) {
          valid = false;
          break;
        }
      }

      if (!valid)
        continue;

      for (int x : descendents[root])
        --in_degrees[x];

      unordered_map<int, vector<int>> subtree_nodes;
      for (int x : nodes)
        if (x != root)
          subtree_nodes[subtree_set.find(x)].push_back(x);

      // recursively build each subtree.
      for (const auto &sub : subtree_nodes) {
        if (!build_tree(sub.second, root, ans))
          return false;
      }
      ans[root] = parent;
      return true;
    }
  }
  return false;
}

void solve() {
  int n, m;
  cin >> n >> m;

  descendents.clear();
  in_degrees.clear();
  edges.clear();

  descendents.resize(n + 1);
  in_degrees.resize(n + 1);
  subtree_set.makeset(n + 1);

  // build graph
  for (int i = 0; i < m; ++i) {
    int x, y, lca;
    cin >> x >> y >> lca;

    if (x != lca) {
      descendents[lca].push_back(x);
      ++in_degrees[x];
    }

    if (y != lca) {
      descendents[lca].push_back(y);
      ++in_degrees[y];
    }
    edges.emplace_back(x, y, lca);
  }

  vector<int> v(n), parents(n + 1);
  iota(v.begin(), v.end(), 1);

  if (!build_tree(v, 0, parents)) {
    cout << "Impossible";
    return;
  }

  for (int i = 1; i <= n; ++i)
    cout << " " << parents[i];
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