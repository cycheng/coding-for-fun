// [1] https://leetcode.com/problems/largest-rectangle-in-histogram/
// [2] https://ithelp.ithome.com.tw/articles/10217418
//
// #stack, #array
// #like
#include "std.h"

// Stack based, O(n)
int largestRectangleArea_12ms(vector<int> &heights) {
  const int n = heights.size();

  // [key] indexes stack: contains increased height, i.e.
  //     heights[indexes[i]] <  heights[indexes[j]], for all j > i.
  stack<int> indexes;
  int maxArea = 0;
  for (int i = 0; i <= n; ++i) {
    const int curH = i < n ? heights[i] : 0;
    while (!indexes.empty() && heights[indexes.top()] > curH) {
      int h = heights[indexes.top()];
      indexes.pop();
      int l = indexes.empty() ? -1 : indexes.top();
      // [key] start from *i*, so width = i - l - 1
      maxArea = max(maxArea, h * (i - l - 1));
    }
    indexes.push(i);
  }

  return maxArea;
}

int largestRectangleArea_16ms(vector<int> &heights) {
  const int n = heights.size();
  vector<int> indexes;
  indexes.reserve(n);

  int maxArea = 0;
  for (int i = 0; i <= n; ++i) {
    const int curH = i < n ? heights[i] : 0;
    while (!indexes.empty() && heights[indexes.back()] > curH) {
      int h = heights[indexes.back()];
      indexes.pop_back();
      int l = indexes.empty() ? -1 : indexes.back();
      maxArea = max(maxArea, h * (i - l - 1));
    }
    indexes.push_back(i);
  }

  return maxArea;
}

// https://leetcode.com/problems/largest-rectangle-in-histogram/discuss/28902/5ms-O(n)-Java-solution-explained-(beats-96)
// by anton4
//
// Array based, O(n)
int largestRectangleArea_8ms(vector<int> &heights) {
  const int n = heights.size();

  vector<int> lessFromLeft(n), lessFromRight(n);
  lessFromLeft[0] = -1;
  lessFromRight[n - 1] = n;

  for (int i = 1; i < n; ++i) {
    int p = i - 1;
    while (p >= 0 && heights[p] >= heights[i])
      p = lessFromLeft[p];
    lessFromLeft[i] = p;
  }

  for (int i = n - 1; i >= 0; --i) {
    int p = i + 1;
    while (p < n && heights[p] >= heights[i])
      p = lessFromRight[p];
    lessFromRight[i] = p;
  }

  int maxArea = 0;
  for (int i = 0; i < n; ++i) {
    maxArea = std::max(maxArea,
                       heights[i] * (lessFromRight[i] - lessFromLeft[i] - 1));
  }

  return maxArea;
}