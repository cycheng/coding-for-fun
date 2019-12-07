// [1] https://leetcode.com/problems/longest-palindromic-subsequence/
// [2] https://ithelp.ithome.com.tw/articles/10218596
// #dp, #string
#include "std.h"

int longestPalindromeSubseq_64ms(string s) {
  // s[i][j] = s[i+1][j-1]
  // s[i][j] = max(s[i][j-1], s[i+1][j])
  const int n = s.length();
  vector<vector<int>> dp(n, vector<int>(n));
  for (int i = n - 1; i >= 0; --i) {
    dp[i][i] = 1;
    for (int j = i + 1; j < n; ++j) {
      if (s[i] == s[j])
        dp[i][j] = dp[i + 1][j - 1] + 2;
      else
        dp[i][j] = max(dp[i][j - 1], dp[i + 1][j]);
    }
  }
  return dp[0][n - 1];
}

int lcs(const string &s, const string &t) {
  const int m = s.length() + 1, n = t.length() + 1;
  vector<vector<int>> dp(m, vector<int>(n));
  for (int i = 1; i < m; ++i)
    for (int j = 1; j < n; ++j)
      if (s[i - 1] == t[j - 1])
        dp[i][j] = dp[i - 1][j - 1] + 1;
      else
        dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
  return dp[m - 1][n - 1];
}

int longestPalindromeSubseq_116ms(string s) {
  string t = s;
  reverse(s.begin(), s.end());
  return lcs(s, t);
}
