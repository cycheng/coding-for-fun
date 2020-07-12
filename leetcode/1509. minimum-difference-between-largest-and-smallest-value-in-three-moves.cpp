// https://leetcode.com/contest/biweekly-contest-30/problems/minimum-difference-between-largest-and-smallest-value-in-three-moves/
//
// #biweekly 30
// #array, #5, #like

// Given an int array, return the 4th largest difference. (cy: need to prove
// it.)
// ≡ choose one element of nums and change it by any value in one move.
//   Return the minimum difference between the largest and smallest value of
//   nums after perfoming at most 3 moves.
//
// Constraints:
// 1 <= nums.length <= 10^5
// -10^9 <= nums[i] <= 10^9

#include "std.h"

int minDifference(vector<int> &nums) {
  if (nums.size() < 5)
    return 0;
  sort(nums.begin(), nums.end());

  int l = 0, r = nums.size() - 1;
  int mindiff = nums[r] - nums[l];
  // 1st largest diff = nums[r] - nums[l]
  mindiff = min(mindiff, nums[r] - nums[l + 3]);
  mindiff = min(mindiff, nums[r - 1] - nums[l + 2]);
  mindiff = min(mindiff, nums[r - 2] - nums[l + 1]);
  mindiff = min(mindiff, nums[r - 3] - nums[l]);
  return mindiff;
}

// From tmwilliamlin168's contest solution:
//   int ans = 2e9;
//   for (int i = 3, j = nums.size() - 1; ~i; --i, --j)
//     ans = min(nums[j] - nums[i], ans);
