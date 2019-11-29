// [1] https://leetcode.com/problems/distinct-subsequences/
// [2] https://ithelp.ithome.com.tw/articles/10218596
// #dp, #string
#include "std.h"

//     r a b b b i t
//   1 1 1 1 1 1 1 1
// r 0 1 1 1 1 1 1 1
// a 0 0 1 1 1 1 1 1
// b 0 0 0 1 2 3 3 3
// b 0 0 0 0 1 3 3 3
// i 0 0 0 0 0 0 3 3
// t 0 0 0 0 0 0 0 3

//     b a b g b a g
//   1 1 1 1 1 1 1 1
// b 0 1 1 2 2 3 3 3
// a 0 0 1 1 1 1 4 4
// g 0 0 0 0 1 1 1 5

int numDistinct_8ms(string s, string t) {
  const int m = t.length() + 1, n = s.length() + 1;
  vector<vector<int64_t>> dp(m, vector<int64_t>(n));
  for (int i = 0; i < n; ++i)
    dp[0][i] = 1;

  for (int i = 1; i < m; ++i)
    for (int j = 1; j < n; ++j) {
      if (t[i - 1] == s[j - 1])
        dp[i][j] = dp[i][j - 1] + dp[i - 1][j - 1];
      else
        dp[i][j] = dp[i][j - 1];
    }

#ifdef DUMP
  cout << "    ";
  for (int i = 1; i < n; ++i)
    cout << s[i - 1] << " ";
  cout << endl;
  for (int i = 0; i < m; ++i) {
    if (i)
      cout << t[i - 1];
    else
      cout << " ";

    for (int j = 0; j < n; ++j)
      cout << " " << dp[i][j];

    cout << endl;
  }
#endif

  return dp[m - 1][n - 1];
}