
// [1] https://leetcode.com/problems/shortest-common-supersequence/submissions/
// [2] https://ithelp.ithome.com.tw/articles/10218596
//
// #dp, #string
// #like
#include "std.h"

string shortestCommonSupersequence_20ms(string s, string t) {
  const int m = s.length() + 1, n = t.length() + 1;
  vector<vector<int>> dp(m, vector<int>(n));
  for (int i = 1; i < m; ++i)
    for (int j = 1; j < n; ++j)
      if (s[i - 1] == t[j - 1])
        dp[i][j] = dp[i - 1][j - 1] + 1;
      else
        dp[i][j] = max(dp[i][j - 1], dp[i - 1][j]);

  string ans;
  int i = m - 1, j = n - 1;
  while (i && j) {
    if (s[i - 1] == t[j - 1]) {
      ans += s[i - 1];
      --i, --j;
    } else {
      if (dp[i][j - 1] > dp[i - 1][j]) {
        ans += t[j - 1];
        --j;
      } else {
        ans += s[i - 1];
        --i;
      }
    }
  }

  while (i)
    ans += s[i - 1], --i;
  while (j)
    ans += t[j - 1], --j;
  reverse(ans.begin(), ans.end());

  return ans;
}