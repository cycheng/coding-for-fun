// https://leetcode.com/contest/biweekly-contest-32/problems/minimum-insertions-to-balance-a-parentheses-string/
//
// #biweekly 32
// #4, #string, #stack

// Given a parentheses string "))((())())", find the minimum insertions to
// balance it.
//
// A parentheses string is balanced if:
// '(' have a corresponding two consecutive '))'

// Constraints:
// 1 <= s.length <= 10^5
// s consists of '(' and ')' only.

#include "std.h"
int minInsertions(string s) {
  int right = 0, ans = 0;
  for (int i = 0; i < s.size(); ++i) {
    char c = s[i];
    if (c == '(') {
      right += 2;
    } else {
      // append ')'
      if (i + 1 == s.size() || s[i + 1] != ')')
        ++ans;
      else
        ++i;
      right -= 2;
      // append '('
      if (right < 0) {
        ++ans;
        right += 2;
      }
    }
  }
  return ans + right;
}