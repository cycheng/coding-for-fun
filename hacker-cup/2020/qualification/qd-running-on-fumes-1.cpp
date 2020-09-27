// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/D1
#include <bits/stdc++.h>
using namespace std;

struct cost {
  uint64_t totalcost = 0;
  int idx = 0;
  cost() = default;
  cost(uint64_t c, int idx) : totalcost(c), idx(idx) {}
};

int64_t solve() {
  int n, m;
  cin >> n >> m;
  vector<int> costs(n);
  for (int i = 0; i < n; ++i)
    cin >> costs[i];

  auto greaterCost = [](const cost &a, const cost &b) {
    return a.totalcost > b.totalcost;
  };
  // top is the smallest cost.
  priority_queue<cost, vector<cost>, decltype(greaterCost)> pq(greaterCost);

#ifdef DEBUG_DUMP
  cout << endl;
#endif
  // uint64_t ans = 1e9;
  int curm = m + 1;
  uint64_t curcost = 0;
  for (int i = 0; i < n; ++i) {

    --curm;
#ifdef DEBUG_DUMP
    cout << "  " << i << " curm, curcost = " << curm << " " << curcost << endl;
#endif
    if (curm < 0) {
      while (!pq.empty() && pq.top().idx < i - m)
        pq.pop();

      if (pq.empty())
        return -1;
#ifdef DEBUG_DUMP
      cout << "    top cost, id = " << pq.top().totalcost << " " << pq.top().idx
           << endl;
#endif
      curcost = pq.top().totalcost;
      curm = m - (i - pq.top().idx);
    }
    if (costs[i])
      pq.emplace(curcost + costs[i], i);
  }
  return curcost;
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