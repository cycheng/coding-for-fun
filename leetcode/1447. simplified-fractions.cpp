// https://leetcode.com/contest/biweekly-contest-26/problems/simplified-fractions/
//
// #biweekly 26
// #gcd, #4
#include "std.h"

// given n, list all simplified fractions with denominator = 2..n
//
// Constraints:
// 1 <= n <= 100

// 108 ms.
vector<string> simplifiedFractions(int n) {
  //             = 1 .. n-1
  // denominator = 2 .. n
  vector<string> ans;
  for (int d = 2; d <= n; ++d)
    for (int i = 1; i < d; ++i)
      if (__gcd(i, d) == 1)
        ans.push_back(to_string(i) + "/" + to_string(d));
  return ans;
}
