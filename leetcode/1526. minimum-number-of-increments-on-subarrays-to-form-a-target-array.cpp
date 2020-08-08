// https://leetcode.com/contest/biweekly-contest-31/problems/minimum-number-of-increments-on-subarrays-to-form-a-target-array/
//
// #biweekly 31
// #7, #sub array

// Given target, e.g. [1 2 3 1 2 3], find the minimum steps to increase
// sub-array of [0 0 0 0 0 0] to target.
//
// In each step, choose a subarray, and increase only 1.
//
// Constraints:
// 1 <= target.length <= 10^5
// 1 <= target[i] <= 10^5

#include "std.h"

int minNumberOperations(vector<int> &target) {
  // find increasing subarray.
  int prev = 0, base = 0;
  int ans = 0;
  for (int cur : target) {
    if (cur < prev) {
      ans += prev - base;
      base = cur;
    }
    prev = cur;
  }
  ans += prev - base;
  return ans;
}