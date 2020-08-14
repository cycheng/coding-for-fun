// https://leetcode.com/contest/biweekly-contest-32/problems/minimum-insertions-to-balance-a-parentheses-string/
//
// #biweekly 32
// #6, #dp, #mask, #like

// Given a string S contains only digit character 0 to 9, find the longest
// non-empty substring of s which:
// 1. (must) contains even number count of digit 0 or 1 or .. or 9
// 2. (optional) contains *one* odd number count of digit 0 or 1 or .. or 9
// Constraints:

#include "std.h"

int longestAwesome(string s) {
  constexpr int totalmasks = 1 << 10;
  const int n = s.length();
  // bit 0 to 9 represent digit '0' to '9'
  // bit i = 0: the digit i has even count
  //         1: the digit i has odd count
  int maskpos[totalmasks];
  fill(maskpos, maskpos + totalmasks, n);
  int mask = 0, ans = 0;
  // If we encounter mask = 0, the distance should be 0 to current index
  // i.e. dist = current index + 1
  maskpos[0] = -1;
  for (int i = 0; i < n; ++i) {
    mask ^= 1 << s[i] - '0';
    if (maskpos[mask] == n)
      maskpos[mask] = i;
    // For example: 3 0 2 2 1 1
    // i0  0001   3
    // i1  1001   3 0
    // i2  1011   3 0 2
    // i3  1001   3 0 2 2
    // i4  1101   3 0 2 2 1
    // i5  1001   3 0 2 2 1 1
    //
    // the max range is 2 2 1 1 => [i1, i5)
    ans = max(ans, i - maskpos[mask]);

    // Flip one bit, because one odd is allowed.
    for (int j = 0; j < 10; ++j)
      ans = max(ans, i - maskpos[mask ^ 1 << j]);
  }
  return ans;
}
