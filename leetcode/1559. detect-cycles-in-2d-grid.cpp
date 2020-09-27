// https://leetcode.com/contest/biweekly-contest-33/problems/detect-cycles-in-2d-grid/
//
// #biweekly 33
// #6, #bfs
#include "std.h"

bool bfs_find_circle(vector<vector<char>> &grid,
                     vector<vector<pair<int, int>>> &prev, int row, int col,
                     int m, int n) {
  // row, col
  queue<pair<int, int>> q;
  q.emplace(row, col);
  const char color = grid[row][col];
  // skip visited.
  if (toupper(color) == color)
    return false;
  // cout << "check grid[" << row << "][" << col << "] = " << color << endl;
  while (q.size()) {
    int len = q.size();
    for (int i = 0; i < len; ++i) {
      int r = q.front().first, c = q.front().second;
      q.pop();
      grid[r][c] = toupper(grid[r][c]);

      const vector<pair<int, int>> offsets = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};
      for (const auto &o : offsets) {
        int up = o.first, left = o.second;
        int rr = r + up, cc = c + left;
        if (rr >= 0 && rr < m && cc >= 0 && cc < n &&
            tolower(grid[rr][cc]) == color) {
          if (grid[rr][cc] == tolower(grid[rr][cc])) {
            q.emplace(rr, cc);
            prev[rr][cc] = {r, c};
          } else {
            pair<int, int> prevcoord = prev[r][c];
            if (prevcoord.first != rr || prevcoord.second != cc) {
              // cout << "  checking (" << rr << ", " << cc << "), cycle: grid["
              // << r << "][" << c << "]'s prev = '"
              //     << prevcoord.first << ", " << prevcoord.second << endl;
              return true;
            }
          }
        }
      }
    }
  }
  return false;
}

bool containsCycle(vector<vector<char>> &grid) {
  const int m = grid.size(), n = grid[0].size();
  vector<vector<pair<int, int>>> previous(m, vector<pair<int, int>>(n));
  for (int i = 0; i < m; ++i)
    for (int j = 0; j < n; ++j)
      if (grid[i][j])
        if (bfs_find_circle(grid, previous, i, j, m, n))
          return true;
  return false;
}