// https://leetcode.com/contest/biweekly-contest-33/problems/minimum-numbers-of-function-calls-to-make-target-array/
//
// #biweekly 33
// #5, #greedy, #bit

// func modify(arr, op, idx) {
//   if (op == 0)
//     arr[idx] = arr[idx] + 1
//   if (op == 1)
//     for (i = 0; i < arr.size; ++i)
//       arr[i] = arr[i] * 2
// }

// Use modify function to form an integer array nums from an zeros array.
//
// Return the minimum number of function calls to make nums from arr.
#include "std.h"

int minOperations(vector<int> &nums) {
  int maxp = 0;
  int numOne = 0;
  for (int v : nums) {
    int p = 0;
    while (v) {
      if (v % 2)
        ++numOne;
      v >>= 1;
      maxp = max(maxp, p++);
    }
  }
  return numOne + maxp;
}