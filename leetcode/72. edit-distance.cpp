// [1] https://leetcode.com/problems/edit-distance/
// [2] https://ithelp.ithome.com.tw/articles/10218596
// #dp, #string
#include "std.h"

int minDistance(string word1, string word2) {
  // variation of LCS
  const int n = word1.length() + 1, m = word2.length() + 1;
  vector<vector<int>> dp(m, vector<int>(n));
  for (int i = 1; i < n; ++i)
    dp[0][i] = i;

  for (int i = 1; i < m; ++i) {
    dp[i][0] = i;
    for (int j = 1; j < n; ++j) {
      if (word1[j - 1] == word2[i - 1])
        dp[i][j] = dp[i - 1][j - 1];
      else {
        // dp[i][j - 1]: remove word1[j - 1]
        // dp[i - 1][j]: insert word2[i - 1] to word1[j - 1]
        // dp[i - 1][j - 1]: replace word1[j - 1] with word2[i - 1]
        dp[i][j] = min({dp[i][j - 1], dp[i - 1][j], dp[i - 1][j - 1]}) + 1;
      }
    }
  }
  return dp[m - 1][n - 1];
}
