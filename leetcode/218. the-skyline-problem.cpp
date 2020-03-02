// https://leetcode.com/problems/the-skyline-problem/

#include "std.h"

// 32ms 83.09%
// 15.6 MB (23.08%)
// Ref:
// https://leetcode.com/problems/the-skyline-problem/discuss/61197/(Guaranteed)-Really-Detailed-and-Good-(Perfect)-Explanation-of-The-Skyline-Problem
// and yhatl's c++ implementation.
vector<vector<int>> getSkyline_32ms(vector<vector<int>> &buildings) {
  multiset<pair<int, int>> points;
  for (const auto &b : buildings) {
    points.emplace(b[0], -b[2]);
    points.emplace(b[1], b[2]);
  }

  vector<vector<int>> ans;
  int curSkyline = 0;
  multiset<int> heights({0});
  for (const auto &p : points) {
    // start of a building
    if (p.second < 0)
      heights.emplace(-p.second);
    else // end of a building
         // Note! use 'find' in order to erase one matching point.
         // erase(p.second) erases all elements of value p.second.
      heights.erase(heights.find(p.second));

    if (curSkyline != *heights.rbegin()) {
      curSkyline = *heights.rbegin();
      ans.push_back({p.first, curSkyline});
    }
  }
  return ans;
}

// cy first attempt:
// 48 ms (35.27 %)
// 18.2 mb (7.69 %)
class Solution {
public:
  enum { L = 0, R, H };
  enum { X = 0, Y };
  void print(const vector<vector<int>> &xyAry) {
    for (const auto &v : xyAry)
      cout << v[0] << " " << v[1] << " " << v[2] << endl;
  }
  int64_t getLR(int l, int r) { return ((int64_t)l << 32) | r; }
  vector<vector<int>> getSkyline(vector<vector<int>> &buildings) {
    // height to id with the same height
    unordered_map<int, int> heights;
    int k = 0;
    while (k < buildings.size()) {
      int h = buildings[k][H];
      if (heights.count(h) && buildings[heights[h]][R] >= buildings[k][L]) {
        buildings[heights[h]][R] =
            max(buildings[heights[h]][R], buildings[k][R]);
        buildings.erase(buildings.begin() + k);
        continue;
      }
      heights[h] = k;
      ++k;
    }

    // [L, R] -> [max height id]
    unordered_map<int64_t, int> width;
    k = 0;
    while (k < buildings.size()) {
      int64_t lr = getLR(buildings[k][L], buildings[k][R]);
      if (width.count(lr)) {
        if (buildings[width[lr]][H] < buildings[k][H])
          buildings[width[lr]][H] = buildings[k][H];
        buildings.erase(buildings.begin() + k);
        continue;
      }
      width[lr] = k;
      ++k;
    }

    // print(buildings);

    vector<vector<int>> left;
    // height to id map
    multimap<int, int> h2id;
    // [Li, Ri, Hi]
    int curid = 0;
    for (const auto b : buildings) {
      while (h2id.rbegin() != h2id.rend()) {
        const auto &curHeightest = buildings[h2id.rbegin()->second];
        if (curHeightest[R] <= b[L]) {
          h2id.erase(prev(h2id.end()));
          continue;
        }
        if (curHeightest[H] <= b[H]) {
          if (curHeightest[L] == b[L])
            left.back() = b; //{b[L], b[H]};
          else
            left.push_back(b); //{b[L], b[H]});
        }
        break;
      }

      if (h2id.empty())
        left.push_back(b); //{b[L], b[H]});

      h2id.emplace(b[2], curid);
      ++curid;
    }

    sort(
        buildings.begin(), buildings.end(),
        [](const vector<int> &a, const vector<int> &b) { return a[R] > b[R]; });
    // print(buildings);
    vector<vector<int>> right;
    h2id.clear();
    curid = 0;
    for (const auto b : buildings) {
      // cout << "test: " << b[L] << ", " << b[R] << ", " << b[H] << endl;
      while (h2id.rbegin() != h2id.rend()) {
        const auto &curHeightest = buildings[h2id.rbegin()->second];
        // cout << "cmp: " << curHeightest[L] << ", " << curHeightest[R] << ", "
        // <<
        // curHeightest[H] << endl;
        if (curHeightest[L] > b[R]) {
          h2id.erase(prev(h2id.end()));
          continue;
        }
        if (curHeightest[H] < b[H]) {
          right.push_back({b[R], b[L], curHeightest[H]});
        }
        break;
      }

      if (h2id.empty())
        right.push_back({b[R], b[L], 0});

      h2id.emplace(b[H], curid);
      ++curid;
    }

    reverse(right.begin(), right.end());
    /*left.insert(left.end(), right.begin(), right.end());
    sort(left.begin(), left.end(),
         [](const vector<int> &a, const vector<int> &b){
           return a[L] < b[L];
         });*/

    k = left.size() - 1;
    while (k > 0) {
      if (left[k][H] == left[k - 1][H] && left[k][L] == left[k - 1][R])
        left.erase(left.begin() + k);
      --k;
    }

    cout << "right:" << endl;
    print(right);
    k = right.size() - 1;
    while (k > 0) {
      if ((right[k][H] == right[k - 1][H] && right[k][R] == right[k - 1][L])
          //|| (right[k][R] == right[k - 1][R])
      ) {
        right[k][R] = right[k - 1][R];
        right.erase(right.begin() + k - 1);
      }
      if (right[k][L] == right[k - 1][L]) {
        right[k][H] = min(right[k][H], right[k - 1][H]);
        right.erase(right.begin() + k - 1);
      }
      --k;
    }

    // cout << "left:" << endl; print(left);
    // cout << "right:" << endl; print(right);
    vector<vector<int>> merged;
    merged.reserve(left.size() + right.size());
    int i = 0, j = 0;
    while (i < left.size() && j < right.size()) {
      if (left[i][X] < right[j][X]) {
        // merged.push_back(left[i++]);
        merged.push_back({left[i][L], left[i][H]});
        ++i;
      } else if (left[i][X] > right[j][X]) {
        // merged.push_back(right[j++]);
        merged.push_back({right[j][L], right[j][H]});
        ++j;
      } else {
        merged.push_back({left[i][L], left[i][H]});
        // duplicated X is removed automatically.
        ++i, ++j;
      }
    }
    while (i < left.size()) {
      merged.push_back({left[i][L], left[i][H]});
      i++;
    }
    while (j < right.size()) {
      merged.push_back({right[j][L], right[j][H]});
      j++;
    }
    return merged;
  }
};
