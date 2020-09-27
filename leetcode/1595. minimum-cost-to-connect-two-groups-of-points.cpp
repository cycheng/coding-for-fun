// https://leetcode.com/contest/weekly-contest-207/problems/minimum-cost-to-connect-two-groups-of-points/
//
// #weekly 207
// #6, #dp, #bitmask, #bipartite graph, #minimum-cost edge cover, #like

//
#include "std.h"

int connectTwoGroups(vector<vector<int>> &cost) {
  const int m = cost.size(), n = cost[0].size();
  const int numMasks = 1 << n;
  const int fullMask = numMasks - 1;
  constexpr int maxValue = 2000;
  vector<int> dpMinCosts(numMasks, maxValue);
  dpMinCosts[0] = 0;

  // For each point of group 1, tentatively connects to each point of group 2
  // and compute the cost.
  for (int group1 = 0; group1 < m; ++group1) {
    // Note! Each iteration set tmp dp to 'maxValue'. Can't eliminate this tmp
    // dp array.
    vector<int> tmpDpMinCosts(numMasks, maxValue);
    for (int submask = 0; submask <= fullMask; ++submask) {
      // if (__builtin_popcount(submask) > group1 + 1)
      //  continue;
      //
      // why we don't check bit count? Because a group 1 point may connect to
      // multiple group 2 point, and multiple group 1 points can connect to the
      // same point of group 2. e.g. [[1,3,5],[4,1,1],[1,5,3]],
      // After the first iteration:
      //    (masks)  000 001 010 011 100 101 110 111
      //    (costs) 2000   1   3   4   5   6   8   9
      //
      // Because point0 of group1 can connect to all point
      for (int group2 = 0; group2 < n; ++group2) {
        // tentatively connect group1 to this group2 point
        int connectMask = submask | (1 << group2);
        tmpDpMinCosts[connectMask] =
            min(tmpDpMinCosts[connectMask],
                min(dpMinCosts[submask], tmpDpMinCosts[submask]) +
                    cost[group1][group2]);
      }
    }
    dpMinCosts.swap(tmpDpMinCosts);
#if 0
    constexpr int numBit = 8;
    for (int i = 0; i <= fullMask; ++i)
      cout << std::bitset<numBit>(i) << " ";
    cout << endl;
    for (int i = 0; i <= fullMask; ++i)
      cout << setw(numBit) << dpMinCosts[i] << " ";
    cout << endl;
#endif
  }
  return dpMinCosts[fullMask];
}