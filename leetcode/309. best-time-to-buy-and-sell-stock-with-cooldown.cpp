// [1]
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/
// [2] https://ithelp.ithome.com.tw/articles/10220050
// #dp, #schedule
// #like
#include "std.h"

// method 1:
// dp(k) = the maximum profit if we sell stock on kth day.
//
// dp(k) = max(prices[k] - prices[x] + dp(0) .. dp(x - 2))
// x = 0 .. k - 1
int maxProfit_72ms(vector<int> &prices) {
  if (prices.size() < 2)
    return 0;

  const int n = prices.size();
  vector<int> dp(n);
  vector<int> leftMax(n);

  for (int k = 1; k < n; ++k) {
    for (int x = k - 1; x >= 0; --x) {
      int prevMax = x >= 2 ? leftMax[x - 2] : 0;
      dp[k] = max(dp[k], prices[k] - prices[x] + prevMax);
    }
    leftMax[k] = max(leftMax[k - 1], dp[k]);
  }
  return leftMax[n - 1];
}

int maxProfit_60ms(vector<int> &prices) {
  if (prices.size() < 2)
    return 0;

  const int n = prices.size();
  vector<int> leftMax(n);
  for (int k = 1; k < n; ++k) {
    int maxk = 0;
    for (int x = k - 1; x >= 0; --x) {
      int prevMax = x >= 2 ? leftMax[x - 2] : 0;
      maxk = max(maxk, prices[k] - prices[x] + prevMax);
    }
    leftMax[k] = max(leftMax[k - 1], maxk);
  }
  return leftMax.back();
}
