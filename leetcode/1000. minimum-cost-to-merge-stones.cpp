// [1] https://leetcode.com/problems/minimum-cost-to-merge-stones/
// [2] https://ithelp.ithome.com.tw/articles/10219072
// [3] https://leetcode.com/problems/minimum-cost-to-merge-stones/discuss/
//     247567/JavaC%2B%2BPython-DP
// #dp, #stone, #3d dp
// #like
#include "std.h"

class Solution_0_or_4ms {
public:
  vector<int> prefixSum;
  int table[31][31][31];

  int K;
  int setv(int i, int j, int m, int v) {
    // cout << i << " " << j << " " << m << " = " << v << endl;
    table[i][j][m] = v;
    return v;
  }
  int dp(int i, int j, int m) {
    int ret;

    if (i == j) {
      if (m == 1)
        ret = 0;
      else
        ret = -1;
      return setv(i, j, m, ret);
    }

    if (table[i][j][m])
      return table[i][j][m];

    if (m == 1) {
      ret = dp(i, j, K) + prefixSum[j + 1] - prefixSum[i];
      return setv(i, j, m, ret);
    }

    int minv = INT_MAX;
    for (int mid = i; mid < j; mid += K - 1) {
      int r1 = dp(i, mid, 1);
      int r2 = dp(mid + 1, j, m - 1);
      if (r1 >= 0 && r2 >= 0)
        minv = min(minv, r1 + r2);
    }
    ret = minv == INT_MAX ? -1 : minv;
    return setv(i, j, m, ret);
  }

  int mergeStones(vector<int> &stones, int K) {
    const int n = stones.size();
    if ((n - 1) % (K - 1))
      return -1;

    memset(table, 0, sizeof(int) * 31 * 31 * 31);
    this->K = K;
    prefixSum.resize(n + 1);
    for (int i = 0; i < n; ++i)
      prefixSum[i + 1] = prefixSum[i] + stones[i];
    return dp(0, n - 1, 1);
  }
};

int mergeStones_4ms(vector<int> &stones, int K) {
  int dp[31][31];
  int n = stones.size();
  if ((n - 1) % (K - 1))
    return -1;

  vector<int> prefixSum(n + 1);
  for (int i = 0; i < n; i++)
    prefixSum[i + 1] = prefixSum[i] + stones[i];

  memset(dp, 0, sizeof(dp));
  for (int from = K - 1; from < n; ++from) {
    for (int i = 0, j = from; i < n - from; ++i, ++j) {
      // for (int j = from; j < n; ++j) {
      dp[i][j] = INT_MAX;
      for (int mid = i; mid < j; mid += K - 1)
        dp[i][j] = min(dp[i][j], dp[i][mid] + dp[mid + 1][j]);
      if ((j - i) % (K - 1) == 0)
        dp[i][j] += prefixSum[j + 1] - prefixSum[i];

      // cout << i << ", " << j << " = " << dp[i][j] << endl;
      //}
    }
  }
  return dp[0][n - 1];
}

// [3,5,1,2,6,1,2,3,2]
// 3
//
// Execution steps for mergeStones_4ms
//
// dp[i,j]       dp table
// 0, 2 = 9        0 1 2 3 4 5 6 7 8
// 1, 3 = 8      0 x x -
// 2, 4 = 9      1 x x x -
// 3, 5 = 9      2 x x x x -
// 4, 6 = 9      3 x x x x x -
// 5, 7 = 6      4 x x x x x x -
// 6, 8 = 7      5 x x x x x x x -
//               6 x x x x x x x x -
//
// 0, 3 = 8        0 1 2 3 4 5 6 7 8
// 1, 4 = 8      0 x x - -
// 2, 5 = 9      1 x x x - -
// 3, 6 = 9      2 x x x x - -
// 4, 7 = 6      3 x x x x x - -
// 5, 8 = 6      4 x x x x x x - -
//               5 x x x x x x x - -
//               6 x x x x x x x x -
//
// 0, 4 = 25       0 1 2 3 4 5 6 7 8
// 1, 5 = 23     0 x x - - -
// 2, 6 = 21     1 x x x - - -
// 3, 7 = 20     2 x x x x - - -
// 4, 8 = 20     3 x x x x x - - -
//               4 x x x x x x - - -
//               5 x x x x x x x - -
//               6 x x x x x x x x -
//
// 0, 5 = 18       0 1 2 3 4 5 6 7 8
// 1, 6 = 17     0 x x - - - -
// 2, 7 = 15     1 x x x - - - -
// 3, 8 = 16     2 x x x x - - - -
//               3 x x x x x - - - -
//               4 x x x x x x - - -
//               5 x x x x x x x - -
//               6 x x x x x x x x -
//
// 0, 6 = 37       0 1 2 3 4 5 6 7 8
// 1, 7 = 34     0 x x - - - - -
// 2, 8 = 32     1 x x x - - - - -
//               2 x x x x - - - - -
//               3 x x x x x - - - -
//               4 x x x x x x - - -
//               5 x x x x x x x - -
//               6 x x x x x x x x -
//
// 0, 7 = 29       0 1 2 3 4 5 6 7 8
// 1, 8 = 28     0 x x - - - - - -
//               1 x x x - - - - - -
//               2 x x x x - - - - -
//               3 x x x x x - - - -
//               4 x x x x x x - - -
//               5 x x x x x x x - -
//               6 x x x x x x x x -
//
// 0, 8 = 50       0 1 2 3 4 5 6 7 8
//               0 x x - - - - - - -
//               1 x x x - - - - - -
//               2 x x x x - - - - -
//               3 x x x x x - - - -
//               4 x x x x x x - - -
//               5 x x x x x x x - -
//               6 x x x x x x x x -
