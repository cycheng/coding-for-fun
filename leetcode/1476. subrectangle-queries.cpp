// https://leetcode.com/contest/biweekly-contest-28/problems/subrectangle-queries/
//
// #biweekly 28
// #stack
#include "std.h"

/**
 * Your SubrectangleQueries object will be instantiated and called as such:
 * SubrectangleQueries* obj = new SubrectangleQueries(rectangle);
 * obj->updateSubrectangle(row1,col1,row2,col2,newValue);
 * int param_2 = obj->getValue(row,col);
 */

// Constraints:

// There will be at most 500 operations considering both methods:
//     updateSubrectangle and getValue.
// 1 <= rows, cols <= 100
// rows == rectangle.length
// cols == rectangle[i].length
// 0 <= row1 <= row2 < rows
// 0 <= col1 <= col2 < cols
// 1 <= newValue, rectangle[i][j] <= 10^9
// 0 <= row < rows
// 0 <= col < cols

// First try. Brute force in contest. 84 ms.
class SubrectangleQueries {
public:
  vector<vector<int>> mat;
  SubrectangleQueries(vector<vector<int>> &rectangle) { mat = rectangle; }

  void updateSubrectangle(int row1, int col1, int row2, int col2,
                          int newValue) {
    for (int i = 0; i <= row2 - row1; ++i)
      for (int j = 0; j <= col2 - col1; ++j)
        mat[row1 + i][col1 + j] = newValue;
  }

  int getValue(int row, int col) { return mat[row][col]; }
};

// Stack based. 104 ms.
class SubrectangleQueries_Stack {
public:
  using RectVal = tuple<int, int, int, int, int>;
  vector<vector<int>> mat;
  vector<RectVal> rects;
  SubrectangleQueries(vector<vector<int>> &rectangle) { mat = move(rectangle); }

  void updateSubrectangle(int row1, int col1, int row2, int col2,
                          int newValue) {
    rects.emplace_back(row1, col1, row2, col2, newValue);
  }

  bool intersect(const RectVal &rect, int row, int col) {
    return get<0>(rect) <= row && row <= get<2>(rect) && get<1>(rect) <= col &&
           col <= get<3>(rect);
  }

  int getValue(int row, int col) {
    for (int i = (int)rects.size() - 1; i >= 0; --i)
      if (intersect(rects[i], row, col))
        return get<4>(rects[i]);
    return mat[row][col];
  }
};
