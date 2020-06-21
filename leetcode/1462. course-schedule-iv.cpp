// https://leetcode.com/contest/biweekly-contest-27/problems/course-schedule-iv/
//
// #biweekly 27
// #graph, #topological sort, #bit, #5
#include "std.h"

// prerequisites = [[u,v]] means u -> v (or v dependent on u)
// queries = [[x,y]] means: whether x has any path point to y
//                          (or x is y's ancestor)
//
// Constraints:
//
// 2 <= n <= 100
// 0 <= prerequisite.length <= (n * (n - 1) / 2)
// 0 <= prerequisite[i][0], prerequisite[i][1] < n
// prerequisite[i][0] != prerequisite[i][1]
// The prerequisites graph has no cycles.
// The prerequisites graph has no repeated edges.
// 1 <= queries.length <= 10^4
// queries[i][0] != queries[i][1]

// 68 tests, 324 ms
vector<bool> checkIfPrerequisite(int n, vector<vector<int>> &prerequisites,
                                 vector<vector<int>> &queries) {
  // ref: other's submission with 'dependon' improvement.

  // build dag and input degree
  vector<vector<int>> graph(n);
  vector<int> degree(n);
  for (const auto &edge : prerequisites) {
    graph[edge[0]].push_back(edge[1]);
    ++degree[edge[1]];
  }

  // topological sort
  vector<int> topsort;
  topsort.reserve(n);
  for (int i = 0; i < n; ++i)
    if (degree[i] == 0)
      topsort.push_back(i);

  for (int i = 0; i < topsort.size(); ++i) {
    int u = topsort[i];
    for (int v : graph[u])
      if (--degree[v] == 0)
        topsort.push_back(v);
  }

  // dependon[u].test(v): whether u is dependent on v.
  vector<bitset<128>> dependon(n);
  for (int u : topsort)
    for (int v : graph[u]) {
      dependon[v].set(u);
      dependon[v] |= dependon[u];
    }

  // scan queries and buld answer
  vector<bool> ans;
  ans.reserve(queries.size());
  for (const auto &q : queries)
    ans.push_back(dependon[q[1]].test(q[0]));

  return ans;
}

// Reachable version:
// reverse(topsort.begin(), topsort.end());
// vector<bitset<128>> reachable(n);
// for (int u : topsort)
//   for (int v : graph[u]) {
//     reachable[u].set(v);
//     reachable[u] |= reachable[v];
//   }
//
// vector<bool> ans;
// ans.reserve(queries.size());
// for (const auto &q : queries)
//   ans.push_back(reachable[q[0]].test(q[1]));
