// https://www.facebook.com/codingcompetitions/hacker-cup/2020/round-1/problems/B
// #like, #20, #binary search, #greedy
#include <bits/stdc++.h>
using namespace std;

// N log drivers and M clusters of frozen logs
// The ith log driver is at position Pi (measured in metres)
// The ith cluster of logs is at position Qi
// Multiple log drivers are permitted to occupy the same position
// Every second, each log driver can move one metre to either the left or right,
// or remain stationary.
vector<int> read(int total, int k) {
  vector<int> data(total);
  for (int i = 0; i < k; ++i)
    cin >> data[i];
  int64_t a, b, c, d;
  cin >> a >> b >> c >> d;
  for (int i = k; i < total; ++i)
    data[i] = ((a * data[i - 2] + b * data[i - 1] + c) % d) + 1;
  return data;
}

int solve() {
  int n, m, k, _;
  cin >> n >> m >> k >> _;

  auto driverPos = read(n, k);
  auto logPos = read(m, k);

  sort(driverPos.begin(), driverPos.end());
  sort(logPos.begin(), logPos.end());

  int lower = 0, upper = 500'000'001;
  while (lower < upper) {
    int budget = (lower + upper) / 2;
    int beginLog = 0;
    // Each log driver can't exceed budget.
    for (int i = 0; i < n; ++i) {
      // +---+---+---+
      // Q1  P1  Q2  Q3
      // Let P1 conver Q1 to Q3, the cost =
      // (1) Q3 - Q1 + abs(P1 - Q1), or
      // (2) Q3 - Q1 + abs(P1 - Q3)
      int endLog = beginLog;
      while (endLog < m) {
        int cost = logPos[endLog] - logPos[beginLog] +
                   min(abs(logPos[beginLog] - driverPos[i]),
                       abs(logPos[endLog] - driverPos[i]));
        if (cost > budget)
          break;
        endLog++;
      }
      // Either cleanup all logs, or leave endLog to the next log driver.
      beginLog = endLog;
    }

    if (beginLog == m)
      upper = budget;
    else
      lower = budget + 1;
  }
  return lower;
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cerr << i << " ";
    cout << "Case #" << i << ": " << solve();
    cout << endl;
  }
  cerr << endl;
  return 0;
}