
#include "std.h"

// [1] https://leetcode.com/problems/maximal-rectangle/
// [2] https://ithelp.ithome.com.tw/articles/10217418
//
int maximalRectangle(vector<vector<char>> &matrix) {
  if (matrix.empty() || matrix[0].empty())
    return 0;

  const int n = matrix[0].size();

  // leftBound[i]: which column is the left side boundary with value = '1'.
  //               it only has meaning when row[i] is '1'
  //
  // rightBound[i]: which column is the right side boundary with value = '0'
  //                it only has meaning when row[i] is '1'
  vector<int> leftBound(n), rightBound(n, n), up(n);

  int maxRect = 0;
  for (const auto &row : matrix) {
    // [Hint]
    //  - Use curLeft, curRight to track boundary of current row.
    //  - Update boundary when row[i] is '1'
    int curLeft = 0;
    for (int i = 0; i < n; ++i) {
      if (row[i] == '1') {
        ++up[i];
        leftBound[i] = max(leftBound[i], curLeft);
      } else {
        curLeft = i + 1;
        up[i] = 0;
        leftBound[i] = 0;
      }
    }

    int curRight = n;
    for (int i = n - 1; i >= 0; --i) {
      if (row[i] == '0') {
        curRight = i;
        rightBound[i] = n;
      } else {
        rightBound[i] = min(rightBound[i], curRight);
      }
    }

    for (int i = 0; i < n; ++i)
      maxRect = max(maxRect, up[i] * (rightBound[i] - leftBound[i]));
  }
  return maxRect;
}