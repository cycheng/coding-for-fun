// https://leetcode.com/contest/biweekly-contest-30/problems/range-sum-of-sorted-subarray-sums/
//
// #biweekly 30
// #array, #priority queue, #4, #like

// Given n elements positive int array. Sort all non-empty sub-arrays in
// increasing order, return sum of the nubmers from index left to right (indexed
// from 1) with mod (1e9 + 7)
//
// Constraints: 1 <= nums.length <= 10^3 nums.length == n 1 <= nums[i] <=
// 100 1 <= left <= right <= n * (n + 1) / 2

#include "std.h"

// O(n^2), 53 tests, 968 ms
int rangeSum(vector<int> &nums, int n, int left, int right) {
  vector<int> subsum;
  subsum.reserve(n * (n + 1) / 2);
  for (int i = 0; i < n; ++i) {
    int s = 0;
    for (int j = i; j < n; ++j) {
      subsum.push_back(nums[j] + s);
      s = subsum.back();
    }
  }

  sort(subsum.begin(), subsum.end());
  int ans = 0;
  int mod = 1e9 + 7;
  for (int i = left - 1; i < right; ++i)
    ans = (ans + subsum[i]) % mod;
  return ans;
}

// Priority queue version. O((right-left) * log n). 53 tests, 16 ms
// see:
// https://leetcode.com/problems/range-sum-of-sorted-subarray-sums/discuss/730511/C%2B%2B-priority_queue-solution
int rangeSum_pq(vector<int> &nums, int n, int left, int right) {
  using sum_id_pair = pair<int, int>;
  priority_queue<sum_id_pair, vector<sum_id_pair>, greater<sum_id_pair>> pq;
  for (int i = 0; i < n; ++i)
    pq.push({nums[i], i + 1});

  const int mod = 1e9 + 7;
  int ans = 0;
  for (int i = 1; i <= right; ++i) {
    auto min_sum = pq.top();
    pq.pop();
    if (i >= left)
      ans = (ans + min_sum.first) % mod;
    if (min_sum.second < n) {
      min_sum.first += nums[min_sum.second++];
      pq.push(min_sum);
    }
  }
  return ans;
}