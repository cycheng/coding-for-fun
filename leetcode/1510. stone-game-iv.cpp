// https://leetcode.com/contest/biweekly-contest-30/problems/stone-game-iv/
//
// #biweekly 30
// #dp, #6

// Given n stones, two players A and B, they take turns to move away stones.
// In each turn, player can take any non-zero square number of stones away.
// If a player cannot make a move, he/she loses the game.
// Assume two players take optimally, return true if A can win the game.
//
// Constraints:
// 1 <= n <= 10^5

#include "std.h"

bool winnerSquareGame(int n) {
  // dp[i] : result of the game when n = i.
  vector<int> dp(n + 1, false);
  // update table from 1 to n.
  for (int i = 1; i <= n; ++i) {
    // search any false result (A loses the game), if there is any false result,
    // A can make B lose the game.
    //
    // Why we only need to check dp[i - j*j]? Because the other cases are:
    // (1) Not under A's control
    // (2) Impossible to reach that state.
    // e.g. i = 7,
    //      j = 1, dp[6]
    //      j = 2, dp[3]
    //      Can A reach dp[5]? Depends on B.
    //      dp[4] A can't reach to this state if i = 7.
    //      dp[2] A can't reach to this state if i = 7.
    for (int j = 1; j * j <= i; ++j)
      if (dp[i - j * j] == false) {
        dp[i] = true;
        break;
      }
  }
  return dp[n];
}