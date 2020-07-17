// https://leetcode.com/contest/biweekly-contest-25/problems/number-of-ways-to-wear-different-hats-to-each-other/
//
// #biweekly 25
// #dp, #7, #like

// Number of ways of n people wear m different hats.
// hats[i] = person i's hat preference
// All people wear different hats
//
// Constraints:
// n == hats.length
// 1 <= n <= 10
// 1 <= hats[i].length <= 40
// 1 <= hats[i][j] <= 40
// hats[i] contains a list of unique integers.

#include "std.h"

int numberWays(vector<vector<int>> &hats) {
  const int n = hats.size();
  const int nummask = 1 << n;
  const int fullmask = nummask - 1;
  // Build hat to people list
  vector<vector<int>> hat2people(40);
  for (int p = 0; p < n; ++p)
    for (int h : hats[p])
      hat2people[h - 1].push_back(p);

  // bit value: 0 = no hat, 1 = wear a hat
  // dp[mask]: number of ways to wear hats under mask state.
  vector<int> dp(nummask);
  dp[0] = 1;
  const int mod = 1e9 + 7;
  for (int h = 0; h < 40; ++h) {
    if (hat2people[h].empty())
      continue;

    // Because we optimize the space, so we update dp in backward order.
    // Before optimize memory space:
    //      dp[h][m] = dp[h - 1][m] + dp[h - 1][m ^ (1 << p)]
    // After optimize memory space:
    //      dp[m]    = dp[m]        + dp[m ^ (1 << p)]
    // e.g.
    for (int m = fullmask; m >= 0; --m) {
      for (int p : hat2people[h])
        // Both versions are correct.
        // if ((1 << p & m) == 0)
        //   dp[m | (1 << p)] = (dp[m | (1 << p)] + dp[m]) % mod;
        if (m & 1 << p)
          dp[m] = (dp[m] + dp[m ^ 1 << p]) % mod;
    }
  }

  return dp[fullmask];
}