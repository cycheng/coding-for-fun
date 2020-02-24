// [1] https://leetcode.com/problems/house-robber-ii/
// [2] https://ithelp.ithome.com.tw/articles/10221041
//
// #dp
// #like
#include "std.h"

int solve(const vector<int> &v, int begin, int end) {
  const int n = v.size();
  int dp0 = 0, dp1 = 0;
  for (int i = begin; i <= end; ++i) {
    int dp = max(dp1, dp0 + v[i]);
    dp0 = dp1;
    dp1 = dp;
  }
  return dp1;
}

int rob_0ms(vector<int> &v) {
  const int n = v.size();
  if (n < 3) {
    // return *max_element(v.begin(), v.end());
    int maxv = 0;
    for (int e : v)
      maxv = max(maxv, e);
    return maxv;
  }

  // max (pick v[0] + dp[2..n-2], dp[1..n-1])
  return max(v[0] + solve(v, 2, n - 2), solve(v, 1, n - 1));
}