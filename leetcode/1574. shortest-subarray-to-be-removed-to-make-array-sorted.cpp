// https://leetcode.com/contest/biweekly-contest-34/problems/shortest-subarray-to-be-removed-to-make-array-sorted/
//
// #biweekly 34
// #5, #subarray

// Given an integer array, remove a subarray (can be empty) from array such that
// the array is non-decreasing.
#include "std.h"

int findLengthOfShortestSubarray(vector<int> &arr) {
  const int n = arr.size();
  int left = 0, right = n - 1;
  for (int i = 0; i < n - 1; ++i, ++left)
    if (arr[i] > arr[i + 1])
      break;
  for (int i = n - 1; i > 0; --i, --right)
    if (arr[i - 1] > arr[i])
      break;
  if (left >= right)
    return 0;
  int ans = right;
  for (int i = 0; i < left + 1; ++i) {
    while (right < n && arr[i] > arr[right])
      ++right;
    ans = min(ans, right - i - 1);
  }
  return ans;
}
