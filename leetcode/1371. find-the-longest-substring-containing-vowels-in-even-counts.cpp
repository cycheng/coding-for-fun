// https://leetcode.com/problems/find-the-longest-substring-containing-vowels-in-even-counts/
//
// #like
// #biweekly 21
#include "std.h"

int findTheLongestSubstring(string s) {
  int ans = 0;
  int mask = 0;
  // mask -> first occurrency position of this mask.
  // e.g. if mask = 11000 in i (first occurrency), then we have 'a', 'i' are
  // odd, 'u', 'e', 'o' are even. If we encounter mask = 11000 again in j, then
  // we get a valid substring from i to j - 1.
  unordered_map<int, int> firstOccurPos;
  // Initialize firstOccurPos[0] with -1 in case the s doesn't contain 'aiueo'.
  firstOccurPos[0] = -1;
  for (int i = 0; i < s.length(); ++i) {
    switch (s[i]) {
    case 'a': mask ^= 1 << 0; break;
    case 'i': mask ^= 1 << 1; break;
    case 'u': mask ^= 1 << 2; break;
    case 'e': mask ^= 1 << 3; break;
    case 'o': mask ^= 1 << 4; break;
    }
    if (firstOccurPos.count(mask) == 0)
      firstOccurPos[mask] = i;
    else {
      ans = max(ans, i - firstOccurPos[mask]);
    }
  }
  return ans;
}