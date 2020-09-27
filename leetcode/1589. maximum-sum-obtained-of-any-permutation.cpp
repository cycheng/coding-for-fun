// https://leetcode.com/contest/biweekly-contest-35/problems/maximum-sum-obtained-of-any-permutation/
//
// #biweekly 35
// #5, #array, #like

// Given: int nums[n], and int requests[m][2]
// * sum of requests[i] = nums[requests[i][0]] + ... + nums[requests[i][1]]
// * We can permute nums
// Return the maximum sum of sum(requests[0]) + .. sum(requests[m - 1])
#include "std.h"

int maxSumRangeQuery(vector<int> &nums, vector<vector<int>> &requests) {
  const int n = nums.size();
  vector<int> count(n + 1);
  for (const auto r : requests) {
    count[r[0]]++;
    count[r[1] + 1]--;
  }
  for (int i = 1; i < n; ++i)
    count[i] += count[i - 1];
  sort(count.begin(), count.begin() + n);
  sort(nums.begin(), nums.end());
  constexpr int mod = 1e9 + 7;
  int64_t ans = 0;
  for (int i = 0; i < n; ++i)
    ans = (ans + (int64_t)nums[i] * count[i]) % mod;
  return ans;
}
