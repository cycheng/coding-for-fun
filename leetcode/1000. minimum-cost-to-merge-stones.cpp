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
