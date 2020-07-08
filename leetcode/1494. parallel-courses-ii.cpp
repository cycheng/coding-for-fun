// https://leetcode.com/contest/biweekly-contest-29/problems/parallel-courses-ii/
//
// #biweekly 29
// #like, #dp, #bit, #mask, #6

// Given an n courses dependency graph, you can take at most k courses in one
// semester, return minimal number of semesters to take all courses.
//
// Constraints:
// 1 <= n <= 15
// 1 <= k <= n
// 0 <= dependencies.length <= n * (n-1) / 2
// dependencies[i].length == 2
// 1 <= xi, yi <= n
// xi != yi
// All prerequisite relationships are distinct, that is, dependencies[i] !=
// dependencies[j]. The given graph is a directed acyclic graph.

#include "std.h"

// see:
// https://leetcode.com/problems/parallel-courses-ii/discuss/709382/C%2B%2B-O(3n)-bitmask-dynamic-programming-code-with-comments-and-tutorial
// https://cp-algorithms.com/algebra/all-submasks.html

int minNumberOfSemesters(int n, vector<vector<int>> &dependencies, int k) {
  const int numMasks = 1 << n;
  const int fullMask = numMasks - 1;
  // Encode course status into bit stream:
  // - Bit position 0 means the status of the first course, i.e. course 1,
  // - Bit position n-1 = the status of course n
  // - Value 1 at position i means course i+1 has not been selected.

  // dependOn[i]: Course i+1 depend on which courses?
  vector<int> dependOn(n);
  for (const auto &e : dependencies) {
    int course = e[1] - 1;
    int prerequisite = e[0] - 1;
    dependOn[course] |= 1 << prerequisite;
  }

  // preqForMask[mask]: Mask's all prerequisites
  vector<int> preqForMask(numMasks);
  for (int m = 1; m < numMasks; ++m)
    for (int c = 0; c < n; ++c)
      if (m & 1 << c)
        preqForMask[m] |= dependOn[c];

  // The minimal number of semesters of mask 'm', m = 1..fullMask.
  vector<int> dp(numMasks, n + 1);

  dp[fullMask] = 0;
  // Top-down, start from fullMask (all courses have not been selected)
  // O(3^n)
  for (int m = fullMask; m > 0; --m) {
    // haveTaken: bit value 1 means the course has been taken.
    //
    // m's 0 bits means the courses which have been taken. So flip m's bits to
    // get have taken course bits.
    //
    // haveTaken might be invalid, so its dp value will also be invalid (equal
    // to n).
    int haveTaken = ~m;

    // Walk through all sub-masks of m
    for (int s = m;; s = (s - 1) & m) {
      // We try to take some courses from m, but we can't take more than k.
      // e.g. m = 1110, k = 1, then sub-mask can not be: 1000, 0100, 0010, 0000
      if (__builtin_popcount(m) - __builtin_popcount(s) > k) {
        if (s == 0)
          break;
        continue;
      }

      // The course we pick is: m ^ s, e.g. k = 2, m = 1110, s = 1000, so
      // pick = 0110.
      int pick = m ^ s;
      // If we have finished all prerequisities for pick, then we can use this
      // 'pick'.
      if ((haveTaken & preqForMask[pick]) == preqForMask[pick])
        dp[s] = min(dp[s], dp[m] + 1);

      if (s == 0)
        break;
    }
  }
  return dp[0];
}

// (1) Why O(3^n)?
//
// If mask m has k enabled bits, then it will have 2^k submasks.
// So we have a total of C(n, k) combinations.
//
// The total number of combinations for all masks will be:
//    Σ k = 0..n {C(n, k) * 2^k}
// By binomial coefficients
//    Σ k = 0..n {C(n, k) * 2^k} = (1 + 2)^n = 3^n
//
// for (int m=0; m<(1<<n); ++m)    <= C(n, k), k = 0 to n enable bits
//   for (int s=m; s; s=(s-1)&m)   <= 2^k (submasks, k enable bits of m)

// (2) Bottom-up, by Wonter:
//
// // dp[i]: minimum number of semesters of mask i, the set bits are courses
// //        that have not been taken
// vector<int> dp(1 << n, n + 1);
// dp[0] = 0;
// for (int i = 1; i < (1 << n); ++i) {
//   // iterate all submask of mask i, and this mask is the mask of last
//   // semester
//   // see: https://cp-algorithms.com/algebra/all-submasks.html
//   for (int j = i; j; j = (j - 1) & i) {
//     if (__builtin_popcount(j) > k) {
//       continue;
//     }
//
//     int already_taken = i ^ ((1 << n) - 1);
//     if ((already_taken & prerequisites[j]) == prerequisites[j]) {
//       dp[i] = min(dp[i], dp[i ^ j] + 1);
//     }
//   }
// }
//
// return dp[(1 << n) - 1];
