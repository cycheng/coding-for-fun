// [1] https://leetcode.com/problems/interleaving-string/
// [2] https://ithelp.ithome.com.tw/articles/10217918
// [3] https://leetcode.com/problems/interleaving-string/discuss/31888/
//     1ms-tiny-DFS-beats-94.57
// #dp, #dfs, #string
#include "std.h"

bool isInterleave_dp_4ms(string s1, string s2, string s3) {
  const int m = s1.length(), n = s2.length();
  if (m + n != s3.length())
    return false;

  vector<vector<bool>> dp(m + 1, vector<bool>(n + 1));
  dp[0][0] = true;
  for (int i = 0; i <= m; ++i)
    for (int j = 0; j <= n; ++j) {
      if (i > 0 && s1[i - 1] == s3[i + j - 1])
        dp[i][j] = dp[i][j] || dp[i - 1][j];
      if (j > 0 && s2[j - 1] == s3[i + j - 1])
        dp[i][j] = dp[i][j] || dp[i][j - 1];
    }

  // for (int i = 0; i <= m; ++i) {
  //   for (int j = 0; j <= n; ++j)
  //     cout << (dp[i][j] ? 1 : 0) << " ";
  //   cout << endl;
  // }

  return dp[m][n];
}

vector<vector<bool>> invalid;
bool dfs(const string &s1, const string &s2, const string &s3, int i, int j) {
  if (invalid[i][j])
    return false;

  if (i + j == s3.length())
    return true;

  bool valid = false;
  if (i < s1.length() && s1[i] == s3[i + j])
    valid = dfs(s1, s2, s3, i + 1, j);
  if (!valid && j < s2.length() && s2[j] == s3[i + j])
    valid = dfs(s1, s2, s3, i, j + 1);
  invalid[i][j] = !valid;
  return valid;
}

bool isInterleave_dfs_0ms(string s1, string s2, string s3) {
  const int m = s1.length(), n = s2.length();
  if (m + n != s3.length())
    return false;

  invalid.resize(m + 1, vector<bool>(n + 1));
  return dfs(s1, s2, s3, 0, 0);
}