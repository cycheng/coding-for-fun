// [1]
// https://leetcode.com/problems/best-time-to-buy-and-sell-stock-with-cooldown/
// [2] https://ithelp.ithome.com.tw/articles/10220050
// #dp, #schedule
// #like
#include "std.h"

// method 1:
// dp(k) = the maximum profit if we sell stock on the kth day.
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

// method 2:
// dp(k) = the maximum profit on the kth day. (no stock on hand)
// dp2(k) = the maximum profit on the kth day with stock on hand
//
// dp(k) = max(dp(k - 1), prices[k] - dp2[k - 1])
// dp2(k) = max(dp2(k - 1), -prices[k] + dp[k - 2])
int maxProfit_4ms(vector<int> &prices) {
  const int n = prices.size();
  if (n < 2)
    return 0;

  vector<int> dp(n + 2);
  vector<int> dp2(n + 2);
  dp2[0] = dp2[1] = -prices[0];
  for (int i = 2; i < n + 2; ++i) {
    dp[i] = max(dp[i - 1], prices[i - 2] + dp2[i - 1]);
    dp2[i] = max(dp2[i - 1], -prices[i - 2] + dp[i - 2]);
  }
  return dp.back();
}

int maxProfit_4ms_2(vector<int> &prices) {
  // yesterday max sell profit
  int yestSell = 0, beforeYestSell = 0;
  // yesterday max buy profit
  int yestBuy = INT_MIN;
  for (int p : prices) {
    int todaySell = max(yestSell, p + yestBuy);
    int todayBuy = max(yestBuy, -p + beforeYestSell);
    beforeYestSell = yestSell;
    yestSell = todaySell;
    yestBuy = todayBuy;
  }
  return yestSell;
}
