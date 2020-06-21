// https://leetcode.com/contest/biweekly-contest-27/problems/check-if-a-string-contains-all-binary-codes-of-size-k/
//
// #biweekly 27
// #string, #bit, #4
#include "std.h"

// Check If a String Contains All Binary Codes of Size K
// e.g. s = "00110", k = 2
//      "00", "01", "10" and "11". They can be all found as substrings

// 168 ms
bool hasAllCodes(string s, int k) {
  const int expectCount = 1 << k;
  vector<bool> seen(expectCount);
  int count = 0, num = 0, mask = expectCount - 1;
  // (MSB) "00110........" (LSB)
  for (int i = 0; i < s.length(); ++i) {
    num = ((num << 1) & mask) + (s[i] - '0');
    if (i >= k - 1 && !seen[num]) {
      seen[num] = true;
      ++count;
    }
  }
  return count == expectCount;
}