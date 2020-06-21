// https://leetcode.com/contest/biweekly-contest-28/problems/allocate-mailboxes/
//
// #biweekly 28
// #like, #dp, #7

// Allocate k mail boxes between n houses such that total distance between each
// house and its nearest mailbox is minimum.
//
// E.g. houses = [1,4,8,10,20], k = 3, allocate mailboxes in
//      position 3, 9 and 20.
//      min distance = |3-1| + |4-3| + |9-8| + |10-9| + |20-20| = 5
//
// Constraints:
// n == houses.length
// 1 <= n <= 100
// 1 <= houses[i] <= 10^4
// 1 <= k <= n
// Array houses contain unique integers.

#include "std.h"

// Cost table. 69 tests, 40 ms. O(n*n*n + n*n*k)
// ref:
// https://leetcode.com/problems/allocate-mailboxes/discuss/685620/JavaC%2B%2BPython-Top-down-DP-Prove-median-mailbox-O(n3)
class Solution_CostTable {
public:
  static const int mxN = 100;
  static const int mxV = 1e6;
  // cost[i][j]: total distance of putting a box between houses[i] and houses[j]
  int cost[mxN][mxN];
  int dp[mxN][mxN];

  // solve(houses, i, k): the minimum total distance of putting k boxes from
  //                      houses[i] to houses[n - 1]
  //
  // Top-down DP. O(n*k*n): Because we have n*k states, and each state needs up
  // to n search.
  int solve(const vector<int> &h, int left, int k) {
    if (k == 0 && left == h.size())
      return 0;
    if (k == 0 || left == h.size())
      return mxV;
    if (dp[left][k] >= 0)
      return dp[left][k];
    int minv = mxV;
    // Search minimal distance: put one box in [left..i] +
    //                          put k-1 boxes in [i..n-1]
    //
    // dp[left][k] = min { cost[left][i] + dp[i+1][k-1] | i = left..n }
    for (int i = left; i < h.size(); ++i)
      minv = min(minv, cost[left][i] + solve(h, i + 1, k - 1));
    dp[left][k] = minv;
    return minv;
  }

  int minDistance(vector<int> &houses, int k) {
    if (k >= houses.size())
      return 0;
    sort(houses.begin(), houses.end());
    const int n = houses.size();
    // cost table construction: O(n^3)
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) {
        cost[i][j] = 0;
        dp[i][j] = -1;
        for (int x = i; x <= j; ++x)
          cost[i][j] += abs(houses[(i + j) / 2] - houses[x]);
      }
    return solve(houses, 0, k);
  }
};