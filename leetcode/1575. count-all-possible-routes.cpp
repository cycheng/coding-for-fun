// https://leetcode.com/contest/biweekly-contest-34/problems/count-all-possible-routes/
//
// #biweekly 34
// #6, #dp

// Given an locations integer array which contains cost of each location.
// Given start location, finish location, and fuel.
// You are allowed to visit any city more than once (including start and
// finish).
//
// Return number of ways from start to finish before out of fuel.
//
// Constraints:
// 2 <= locations.length <= 100
// 1 <= locations[i] <= 10^9
// All integers in locations are distinct.
// 0 <= start, finish < locations.length
// 1 <= fuel <= 200
#include "std.h"

// dp[i][j]: number of ways from 'start' to i when we have fuel j.
// define dp[start][fuel] = 1
int dp[101][201];
int countRoutes(vector<int> &locations, int start, int finish, int fuel) {
  constexpr int mod = 1e9 + 7;
  memset(dp, 0, sizeof(dp));
  dp[start][fuel] = 1;
  const int n = locations.size();
  for (int f = fuel; f > 0; --f)
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j) {
        int remain = f - abs(locations[i] - locations[j]);
        if (i != j && remain >= 0)
          dp[j][remain] = (dp[j][remain] + dp[i][f]) % mod;
      }
  int ans = 0;
  for (int i = 0; i <= fuel; ++i)
    ans = (ans + dp[finish][i]) % mod;
  return ans;
}