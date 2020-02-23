// Day 11.
// https://ithelp.ithome.com.tw/articles/10220552
// https://leetcode.com/problems/valid-permutations-for-di-sequence/discuss/168278/C%2B%2BJavaPython-DP-Solution-O(N2)
// #dp
// #like
#include "std.h"

static constexpr int mod = 1e9 + 7;
int numPermsDISequence_4ms(string S) {
  // @lee215:
  // dp(i, x): means the number of possible permutations of first i + 1 digits
  // (0..i),
  //           where the i + 1th digit is x + 1th smallest in the rest of unused
  //           digits.
  //
  // @tmt514:
  // if S[i - 1] == 'D': dp(i, x) = dp(i - 1, i - 1) + .. + dp(i - 1, x)
  // if S[i - 1] == 'I': dp(i, x) = dp(i - 1, x - 1) + .. + dp(i - 1, 0)
  const int n = S.size();
  vector<vector<int>> dp(n + 1, vector<int>(n + 1));
  dp[0][0] = 1;
  for (int i = 1; i <= n; ++i) {
    if (S[i - 1] == 'D')
      for (int x = i - 1; x >= 0; --x)
        dp[i][x] = (dp[i][x + 1] + dp[i - 1][x]) % mod;
    else
      for (int x = 1; x <= i; ++x)
        dp[i][x] = (dp[i][x - 1] + dp[i - 1][x - 1]) % mod;
  }
  int sum = 0;
  for (int x = 0; x <= n; ++x)
    sum = (sum + dp[n][x]) % mod;
  return sum;
}