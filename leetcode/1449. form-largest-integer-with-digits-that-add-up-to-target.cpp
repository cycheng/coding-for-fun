// https://leetcode.com/problems/form-largest-integer-with-digits-that-add-up-to-target/
//
// #biweekly 26
// #dp, #string, #7
//                          1  2  3  4  .. 9  <= digits
// Given cost of 9 digits [c1 c2 c3 c4 .. c9], and target
// Find the largest integers with cost summation = target
// A digit can be used many times.
//
//              1 2 3 4 5 6 7 8 9
// e.g. cost = [4,3,2,5,6,7,2,5,5], target = 9
//      summation = 9: 14/41, 18/81, 19/91, 347/../743, ..., 7772
//      largest integer is 7772
#include "std.h"

class Solution {
public:
  vector<string> dp;
  string solve(const vector<int> &cost, int target) {
    if (target < 0)
      return "0";
    if (target == 0)
      return "";
    if (dp[target].size())
      return dp[target];

    string maxn = "0";
    for (int i = 0; i < cost.size(); ++i) {
      int diff = target - cost[i];
      string t = solve(cost, diff);
      if (t == "0")
        continue;
      if (t.length() + 1 >= maxn.length())
        maxn = to_string(i + 1) + t;
    }
    dp[target] = maxn;
    return dp[target];
  }
  string largestNumber(vector<int> &cost, int target) {
    dp = vector<string>(target + 1);
    return solve(cost, target);
  }
};