// https://leetcode.com/contest/biweekly-contest-35/problems/make-sum-divisible-by-p/
//
// #biweekly 35
// #5, #array, #prefix sum, #like

// Given: int nums[n], and divisor p.
// Make sum(nums[0 .. n-1]) divisible by p. It is allowed to remove a sub-array
// of nums, but can't remove the whole nums array.
//
// Return the length of the smallest subarray that you need to remove, or -1 if
// it's impossible.
#include "std.h"

int minSubarray(vector<int> &nums, int p) {
  const int n = nums.size();
  int s = 0;
  for (int v : nums)
    s = (s + v) % p;
  if (s == 0)
    return 0;
  int target = s;
  // <prefix sum % p, index>
  unordered_map<int, int> psumIdx;
  psumIdx[0] = -1;
  s = 0;
  int ans = n;
  for (int i = 0; i < n; ++i) {
    s = (s + nums[i]) % p;
    psumIdx[s] = i;
    // Find a prefix_sum(nums[0..idx]) = complement
    // Because prefix_sum(nums[0..i]) - complement = target
    // If the complement exist, the removed subarray length = i - idx
    int complement = (s - target + p) % p;
    if (psumIdx.count(complement))
      ans = min(ans, i - psumIdx[complement]);
  }
  return ans == n ? -1 : ans;
}
