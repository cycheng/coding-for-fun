// https://ithelp.ithome.com.tw/articles/10220552
// https://leetcode.com/problems/ones-and-zeroes/discuss/95807/0-1-knapsack-detailed-explanation.
// #dp
#include "std.h"

// method 1:
// dp: the maximum number of strings we can get from the first k
//     argument strs using limited i number of '0's and j number of '1's.
//
// dp((m, n), k) = max{dp((m-i, n-j), k-1) + 1, dp((m, n), k-1)}
//
class Solution_712ms {
public:
  vector<int> dp;
  int cSize;
  int rcSize;
  void getZeroOneCount(const string &s, int &i, int &j) {
    i = j = 0;
    for (char c : s) {
      if (c == '0')
        ++i;
      else
        ++j;
    }
  }

  int &get(int m, int n, int k) { return dp[m * rcSize + n * cSize + k]; }

  int solve(const vector<string> &s, int m, int n, int k) {
    if (k < 0)
      return 0;

    int &v = get(m, n, k);
    if (v > 0)
      return v;

    int i, j;
    getZeroOneCount(s[k], i, j);

    if (i > m || j > n)
      v = solve(s, m, n, k - 1);
    else
      v = max(solve(s, m, n, k - 1), solve(s, m - i, n - j, k - 1) + 1);
    return v;
  }

  int findMaxForm(vector<string> &strs, int m, int n) {
    const int k = strs.size();
    ++m, ++n;
    cSize = k;
    rcSize = n * k;
    dp.resize(m * n * k);
    return solve(strs, m - 1, n - 1, k - 1);
  }
};

// method 2:
// https://leetcode.com/problems/ones-and-zeroes/discuss/95814/c%2B%2B-DP-solution-with-comments
// dp[i][j] = the max number of strings that can be formed with
//            i 0's and j 1's from the first few strings up to the current
//            string s
//
// Catch: have to go from bottom right to top left.
//
// Why?
// If a cell in the dp is updated(because s is selected), we should be
// adding 1 to dp[i][j] from the previous iteration (when we were not
// considering s).
//
// If we go from top left to bottom right, we would be using results from this
// iteration => overcounting
int findMaxForm_144ms(vector<string> &strs, int m, int n) {
  int dp[m + 1][n + 1];
  memset(dp, 0, sizeof(dp));
  for (const string &s : strs) {
    int numZeros = 0, numOnes = 0;
    for (char c : s) {
      if (c == '0')
        ++numZeros;
      else
        ++numOnes;
    }

    for (int i = m; i >= numZeros; --i)
      for (int j = n; j >= numOnes; --j)
        dp[i][j] = max(dp[i][j], dp[i - numZeros][j - numOnes] + 1);
  }
  return dp[m][n];
}

// unknow method:
// The code is from submitted code with 8 ms runtime.
struct aux {
  int _zeros{0};
  int _ones{0};
  int _min{0};

  aux(const std::string &str) noexcept {
    for (auto c : str)
      if (c == '0')
        _zeros++;
      else if (c == '1')
        _ones++;
    _min = std::min(_zeros, _ones);
  }

  aux(int zeros, int ones) noexcept
      : _zeros(zeros), _ones(ones), _min(std::min(_zeros, _ones)) {
    // BOOST_ASSERT(_zeros >= 0);
    // BOOST_ASSERT(_ones >= 0);
  }

  bool operator<=(const aux &rhs) const {
    return _zeros <= rhs._zeros && _ones <= rhs._ones;
  }

  bool operator<(const aux &rhs) const {
    return (_zeros < rhs._zeros && _ones <= rhs._ones) ||
           (_zeros <= rhs._zeros && _ones < rhs._ones);
  }

  aux operator-(const aux &rhs) const {
    return aux(_zeros - rhs._zeros, _ones - rhs._ones);
  }

  aux operator+(const aux &rhs) const {
    return aux(_zeros + rhs._zeros, _ones + rhs._ones);
  }

  void operator+=(const aux &rhs) {
    _zeros = _zeros + rhs._zeros;
    _ones = _ones + rhs._ones;
  }
};

int findMaxForm_8ms(const std::vector<std::string> &strs, int m, int n) {
  const auto target = aux(m, n);

  // fill-in the vector
  std::vector<aux> v;
  for (const auto &s : strs) {
    v.emplace_back(s);
  }

  // process
  const auto process = [&](auto cmp_func) {
    std::sort(begin(v), end(v), cmp_func);

    int rc{0};
    aux sum{0, 0};
    for (auto &vi : v) {
      if (target < vi)
        break;
      if (sum + vi <= target) {
        sum += vi;
        rc++;
      }
    }
    return rc;
  };

  const auto rc1 = process(
      [&](const auto &v1, const auto &v2) { return v1._min < v2._min; });

  const auto rc2 = process([&](const auto &v1, const auto &v2) {
    aux v1_aux = target - v1;
    aux v2_aux = target - v2;
    return v1_aux._min > v2_aux._min;
  });

  return std::max(rc1, rc2);
}