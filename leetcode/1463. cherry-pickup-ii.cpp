// https://leetcode.com/contest/biweekly-contest-27/problems/cherry-pickup-ii/
//
// #biweekly 27
// #dp, #6
#include "std.h"

// Given a grid (rows, cols) filled with positive values.
// Put robert #1 in (0, 0), robert #2 in (0, cols-1).
// Given robert in (i, j), next valid position is: (i+1, j-1, j, j+1)
// Find maximum sum the two roberts can gather.
//
// Constraints:
// rows == grid.length
// cols == grid[i].length
// 2 <= rows, cols <= 70
// 0 <= grid[i][j] <= 100

// O(row * col^2), 58 tests, 56 ms
int cherryPickup(vector<vector<int>> &grid) {
  // dp[i][j][k]: maximum sum in row i given robert #1 in (i, j),
  //                                         robert #2 in (i, k)
  // dp[i][j][k] = max { dp[i-1][j-1..j+1][k-1..k+1] + grid[i][j] + grid[i][k] }
  int dp[70][70][70];
  memset(dp, 0, sizeof(dp));
  const int nrow = grid.size(), ncol = grid[0].size();
  dp[0][0][ncol - 1] = grid[0][0] + grid[0][ncol - 1];
  for (int r = 1; r < nrow; ++r) {
    for (int left = 0; left < ncol && left <= r; ++left)
      for (int right = ncol - 1; right >= 0 && right >= ncol - r - 1; --right) {
        int maxv = -1;
        // Search 9 possible positions.
        for (int c0 = left - 1; c0 <= left + 1; ++c0)
          for (int c1 = right - 1; c1 <= right + 1; ++c1)
            if (c0 >= 0 && c1 >= 0 && c0 < ncol && c1 < ncol) {
              int points = grid[r][left];
              if (left != right)
                points += grid[r][right];
              maxv = max(maxv, dp[r - 1][c0][c1] + points);
            }
        dp[r][left][right] = maxv;
      }
  }
  int ans = 0;
  for (int i = 0; i < ncol; ++i)
    for (int j = 0; j < ncol; ++j)
      ans = max(ans, dp[nrow - 1][i][j]);
  return ans;
}