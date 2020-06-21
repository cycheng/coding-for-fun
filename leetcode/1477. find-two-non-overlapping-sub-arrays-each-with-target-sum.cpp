// https://leetcode.com/contest/biweekly-contest-28/problems/find-two-non-overlapping-sub-arrays-each-with-target-sum/
//
// #biweekly 28
// #like, #dp, #sliding window, #5

// Find two non-overlapping sub-arrays of arr each with sum equal target.
// Let n = non-overlapping sub-array count.
// - if n > 2, find two sub-arrays where the sum of length is minimal
// - if n < 2, return -1
//
// Constraints:
// 1 <= arr.length <= 10^5
// 1 <= arr[i] <= 1000
// 1 <= target <= 10^8

#include "std.h"

// Sliding window. 59/59 tests, 232 ms
// ref:
// https://leetcode.com/problems/find-two-non-overlapping-sub-arrays-each-with-target-sum/discuss/686105/JAVA-or-Sliding-window-with-only-one-array-or-No-HasMap
static constexpr int INF = 1e9;
int minSumOfLengths_SlidingWindow(vector<int> &arr, int target) {
  // minLen[i]: The minimum length so far with ending index <= i.
  vector<int> minLen(arr.size(), INF);
  int left = 0, sum = 0, minSoFar = INF;

  // [1,1,1,2,2,2,4,4]
  int ans = INF;
  for (int i = 0; i < arr.size(); ++i) {
    sum += arr[i];
    while (sum > target) {
      sum -= arr[left];
      ++left;
    }

    if (sum == target) {
      if (left > 0 && minLen[left - 1] != INF)
        ans = min(ans, minLen[left - 1] + i - left + 1);
      minSoFar = min(minSoFar, i - left + 1);
    }
    minLen[i] = minSoFar;
  }
  return ans == INF ? -1 : ans;
}
