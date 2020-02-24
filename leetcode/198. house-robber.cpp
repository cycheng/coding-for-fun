// [1] https://leetcode.com/problems/house-robber/
// [2] https://ithelp.ithome.com.tw/articles/10221041
//
// #dp
// #like
#include "std.h"

int rob_4ms(vector<int> &nums) {
  // dp(n): the maximum amount of robbing money from nums[0..n - 1].
  // dp(n) = max(dp(n - 1), nums[n - 1] + dp(n - 2))
  if (nums.empty())
    return 0;

  const int n = nums.size();
  vector<int> dp(n + 1);
  dp[1] = nums[0];
  for (int i = 2; i < n + 1; ++i)
    dp[i] = max(dp[i - 1], dp[i - 2] + nums[i - 1]);
  return dp[n];
}

int rob_4ms_2(vector<int> &nums) {
  // optimize space to O(1)
  int dp_i_1 = 0, dp_i_2 = 0;
  const int n = nums.size();
  for (int i = 0; i < n; ++i) {
    int dp_i = max(dp_i_1, dp_i_2 + nums[i]);
    dp_i_2 = dp_i_1;
    dp_i_1 = dp_i;
  }
  return dp_i_1;
}

int rob_0ms(vector<int> &v) {
  int s2 = 0, s1 = 0;
  for (int i = 0; i < v.size(); i++) {
    if (i % 2 == 0)
      s1 = max(s1 + v[i], s2);
    else
      s2 = max(s2 + v[i], s1);
  }
  return max(s1, s2);
}