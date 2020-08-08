// https://leetcode.com/contest/biweekly-contest-31/problems/number-of-sub-arrays-with-odd-sum/
//
// #biweekly 31
// #like, #prefix sum, #sub array

// Given an array of integers arr. Return the number of sub-arrays with odd sum.
//
// Constraints:
// 1 <= arr.length <= 10^5
// 1 <= arr[i] <= 100

#include "std.h"

int numOfSubarrays(vector<int> &arr) {
  constexpr int mod = 1e9 + 7;
  int numOddPrefix = 0, numEvenPrefix = 0, numOddTotal = 0;
  // For example: array [1 2 3 4 5]
  // i0: (1)
  //     numOddPrefix  -> (1)
  //     numEvenPrefix -> 0
  //     numOddTotal   = (1)
  //
  // i1: (2), (2, 1)
  //     numOddPrefix  -> (2, 1)
  //     numEvenPrefix -> (2)
  //     numOddTotal   += (2, 1)
  //
  // i2: (3), (3, 2), (3, 2, 1)
  //     numOddPrefix  -> (3), (3, 2)
  //     numEvenPrefix -> (3, 2, 1)
  //     numOddTotal   += (3), (3, 2)
  //
  // i3: (4), (4, 3), (4, 3, 2), (4, 3, 2, 1)
  //     numOddPrefix  -> (4, 3), (4, 3, 2)
  //     numEvenPrefix -> (4), (4, 3, 2, 1)
  //     numOddTotal   += (4, 3), (4, 3, 2)
  //
  // i4: (5), (5, 4), (5, 4, 3), (5, 4, 3, 2), (5, 4, 3, 2, 1)
  //     numOddPrefix  -> (5, 4), (5, 4, 3, 2, 1)
  //     numEvenPrefix -> (5, 4, 3), (5, 4, 3, 2)
  //     numOddTotal   += (5, 4), (5, 4, 3, 2, 1)
  for (int i = 0; i < arr.size(); ++i) {
    if (arr[i] % 2) {
      numOddPrefix = 1 + numEvenPrefix;
      numEvenPrefix = i + 1 - numOddPrefix;
    } else {
      numEvenPrefix += 1;
    }
    numOddTotal = (numOddTotal + numOddPrefix) % mod;
  }
  return numOddTotal;
}
