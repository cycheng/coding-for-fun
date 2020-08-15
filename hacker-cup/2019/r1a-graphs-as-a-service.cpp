// https://www.facebook.com/codingcompetitions/hacker-cup/2019/round-1/problems/A
// #dijkstar, #priority queue, #like
#include <bits/stdc++.h>
using namespace std;

using edge = pair<int, int>;
bool verify_by_min_spanning_tree(const vector<vector<edge>> &graph, int start) {
  // Dijkstar Algorithm
  auto greaterWeight = [](const edge &a, const edge &b) {
    return a.second > b.second;
  };

  constexpr int invalid_cost = 1e6 + 1;
  vector<int> min_cost(graph.size(), invalid_cost);
  priority_queue<edge, vector<edge>, decltype(greaterWeight)> pq(greaterWeight);
  pq.emplace(start, 0);
  while (pq.size()) {
    edge e = pq.top();
    pq.pop();

    for (const edge &ve : graph[e.first]) {
      //  If there is shorted path to ve.first through ve.
      if (min_cost[ve.first] > e.second + ve.second) {
        min_cost[ve.first] = e.second + ve.second;
        pq.emplace(ve.first, min_cost[ve.first]);
      }
    }
  }

  for (const edge &e : graph[start])
    if (min_cost[e.first] != e.second)
      return false;

  return true;
}

void solve() {
  int n, m;
  cin >> n >> m;
  // each edge: <node, weight>
  vector<vector<edge>> graph(n);
  vector<vector<edge>> graph_for_print(n);
  for (int i = 0; i < m; ++i) {
    int x, y, z;
    cin >> x >> y >> z;
    --x, --y;
    graph[x].emplace_back(y, z);
    graph[y].emplace_back(x, z);
    graph_for_print[x].emplace_back(y, z);
  }
  // calculate minimal spanning tree for each node to verify whether it is valid
  // or not.
  for (int i = 0; i < n; ++i)
    if (!verify_by_min_spanning_tree(graph, i)) {
      cout << "Impossible" << endl;
      return;
    }

  // dump graph.
  int num_edges = 0;
  for (const auto &edges : graph_for_print)
    num_edges += edges.size();
  cout << num_edges << endl;
  for (int i = 0; i < (int)graph_for_print.size(); ++i) {
    for (const edge &e : graph_for_print[i])
      cout << i + 1 << " " << e.first + 1 << " " << e.second << endl;
  }
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cout << "Case #" << i << ": ";
    solve();
  }
  return 0;
}