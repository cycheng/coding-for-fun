// https://adventofcode.com/2023/day/3

#include <bits/stdc++.h>
using namespace std;

/* format
Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
*/

/*
dp[i][j] = instances of all card j for card 0 to card j

*/

void solve(const string &s, vector<vector<int>> &dp, vector<int> &dpsum,
           uint32_t curRow, uint32_t numRow) {
  size_t prefixEndPos = s.find_first_of(':') + 1;
  size_t barPos = s.find_first_of('|');
  string win_str = s.substr(prefixEndPos, barPos - prefixEndPos);
  string num_str = s.substr(barPos + 1);
  unordered_set<int> winset;
  unordered_set<int> numset;

  auto parsenums = [](unordered_set<int> &nset, string &nstr) {
    bool hasnum = false;
    for (uint32_t i = nstr.length(); i > 0; --i) {
      uint32_t idx = i - 1;
      if (!isdigit(nstr[idx])) {
        if (hasnum) {
          int val = strtol(nstr.c_str() + idx, nullptr, 10);
          nset.emplace(val);
          hasnum = false;
          nstr[idx] = '\0';
        }
      } else {
        hasnum = true;
      }
    }
  };
  parsenums(winset, win_str);
  parsenums(numset, num_str);
  uint32_t numMatched = 0;

  for (auto &niter : numset)
    if (winset.count(niter))
      numMatched++;

  // for (uint32_t i = 0; i <= numMatched; ++i)
  //   dp[curRow][curRow + i] = 1;
  dp[curRow][curRow] = 1;

  for (uint32_t i = 1; curRow + i < numRow; ++i) {
    uint32_t sum = 0;
    for (uint32_t j = 1; j <= i; ++j) {
      if (curRow + j <= curRow + numMatched)
        sum += dp[curRow + j][curRow + i];
    }
    dp[curRow][curRow + i] = sum;
  }

#if 0
  cout << "----------------------\n";
  for (auto &iter : dp) {
    for (int v : iter)
      cout << v << " ";
    cout << "\n";
  }
  cout << "\n";
#endif
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  vector<string> ctx;
  ctx.reserve(2048);
  for (std::string line; getline(input, line);)
    ctx.push_back(line);

  int numRows = ctx.size();
  vector<vector<int>> dp(numRows + 1, vector<int>(numRows + 1));

  vector<int> dpSum(numRows + 1);
  for (uint32_t i = numRows; i > 0; --i)
    solve(ctx[i - 1], dp, dpSum, i - 1, numRows);

  uint64_t sum = 0;
  for (auto &iter : dp)
    sum += accumulate(iter.begin(), iter.end(), 0);
  // sum += numRows;

  cout << sum << "\n";
#if 0
  cout << "----------------------\n";

  for (auto &iter : dp) {
    for (int v : iter)
      cout << v << " ";
    cout << "\n";
  }

  cout << "----------------------\n";
  for (int v : dpSum)
    cout << v << " ";

  cout << "\n";
#endif
  return 0;
}
