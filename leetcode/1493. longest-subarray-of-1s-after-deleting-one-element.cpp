// https://leetcode.com/contest/biweekly-contest-29/problems/longest-subarray-of-1s-after-deleting-one-element/
//
// #biweekly 29
// #sliding window, #like, #5

// Given an int array with only value {0, 1}, we need to delete one number from
// it, and return the size of the longest non-empty subarray containing only 1.
//
// Constraints:
// 1 <= nums.length <= 10^5
// nums[i] is either 0 or 1.

#include "std.h"

// cy in contest
int longestSubarray_dp(vector<int> &nums) {
  const int n = nums.size();
  vector<int> dp(n);
  int c = 0;
  for (int i = 0; i < n; ++i) {
    if (nums[i] == 1)
      ++c;
    else
      c = 0;
    dp[i] = c;
  }

  c = 0;
  int maxlen = 0;
  for (int i = n - 1; i > 0; --i) {
    if (nums[i] == 1)
      ++c;
    else {
      maxlen = max(maxlen, c + dp[i - 1]);
      c = 0;
    }
  }
  maxlen = max(maxlen, c);
  return maxlen;
}

// see: lee215
// https://leetcode.com/problems/longest-subarray-of-1s-after-deleting-one-element/discuss/708112/JavaC%2B%2BPython-Sliding-Window-at-most-one-0
int longestSubarray(vector<int> &nums) {
  int i, k = 1, j = 0;
  for (i = 0; i < nums.size(); ++i) {
    if (nums[i] == 0)
      --k;
    if (k < 0)
      if (nums[j++] == 0)
        ++k;
  }
  return i - j - 1;
}
