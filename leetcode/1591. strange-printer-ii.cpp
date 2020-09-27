// https://leetcode.com/contest/biweekly-contest-35/problems/strange-printer-ii/
//
// #biweekly 35
// #6, #topological order, #greedy, #like

// Given a [m x n] matrix, the grid value means color.
//
// Each turn, we can print a solid rectangular of a color in the matrix.
// The same color cannot be used again.
//
// Given a [m x n] matrix targetGrid, return true if it doesn't volatile the
// above rule, else return false.
#include "std.h"

enum {
  UNINIT,
  VISITED,
  CHECKED,
};
bool dfsHasLoop(const unordered_set<int> edges[61], vector<int> &state,
                int color) {
  state[color] = VISITED;
  for (int v : edges[color]) {
    if (state[v] == CHECKED)
      continue;
    if (state[v] == VISITED)
      return true;
    if (dfsHasLoop(edges, state, v))
      return true;
  }
  state[color] = CHECKED;
  return false;
}

bool isPrintable(vector<vector<int>> &targetGrid) {
  const int m = targetGrid.size(), n = targetGrid[0].size();
  unordered_set<int> edges[61];
  // greedy, O(CMN)
  for (int color = 1; color <= 60; ++color) {
    int top = m, left = n, right = 0, bottom = 0;
    for (int i = 0; i < m; ++i)
      for (int j = 0; j < n; ++j)
        if (color == targetGrid[i][j]) {
          top = min(top, i);
          left = min(left, j);
          right = max(right, j);
          bottom = max(bottom, i);
        }

    // find overlapped grid
    for (int i = top; i <= bottom; ++i)
      for (int j = left; j <= right; ++j)
        if (color != targetGrid[i][j])
          edges[color].insert(targetGrid[i][j]);
  }

  // dfs check loop
  vector<int> state(61);
  for (int color = 1; color <= 60; ++color)
    if (dfsHasLoop(edges, state, color))
      return false;
  return true;
}