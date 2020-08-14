// https://leetcode.com/contest/biweekly-contest-32/problems/kth-missing-positive-number/
//
// #biweekly 32
// #3, #array

// Constraints:
// 1 <= arr.length <= 1000
// 1 <= arr[i] <= 1000
// 1 <= k <= 1000
// arr[i] < arr[j] for 1 <= i < j <= arr.length

#include "std.h"

// Given a strictly increasing order int array, find the Kth missing number
int findKthPositive(vector<int> &arr, int k) {
  int cnt = 0;
  for (int i = 1; i <= arr.size(); ++i) {
    int a = arr[i - 1];
    ++cnt;
    while (a > cnt) {
      if (k == 1)
        return cnt;
      ++cnt;
      --k;
    }
  }
  return cnt + k;
}