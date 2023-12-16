// https://adventofcode.com/2023/day/12

#include <bits/stdc++.h>
using namespace std;

/* format
???.### 1,1,3

DP soluction, ref:
https://www.reddit.com/r/adventofcode/comments/18ge41g/2023_day_12_solutions/

@AndreiaCo:
dp[i][j]: number of possible arrangements by matching the groups on the last i
          groups with the substring starting at j and continuing until the end.

                  0   1   2   3   4   5   6   7   8
  +-------------+---+---+---+---+---+---+---+---+---+
  |             | ? | ? | ? | . | # | # | # | . | _ |
  +=============+===+===+===+===+===+===+===+===+===+
0 | [ ]         | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1 | 1 |
1 | [ 3 ]       | 1 | 1 | 1 | 1 | 1 | 0 | 0 | 0 | 0 |
2 | [ 1; 3 ]    | 3 | 2 | 1 | 0 | 0 | 0 | 0 | 0 | 0 |
3 | [ 1; 1; 3 ] | 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 |
  +-------------+---+---+---+---+---+---+---+---+---+

The last column is a dummy.

*/
uint64_t solve(string &record, vector<uint32_t> groups) {
  size_t numCol = record.length(), numRow = groups.size();
  vector<vector<uint64_t>> dp(numRow, vector<uint64_t>(numCol, 0));

  // init the first row, which has empty group.
  for (size_t i = numCol; i > 0; --i) {
    if (record[i - 1] == '.' || record[i - 1] == '?')
      dp[0][i - 1] = 1;
    else
      break;
  }

  for (int i = 1; i < (int)groups.size(); ++i) {
    uint32_t groupLen = groups[i];

    for (int j = (int)numCol - 2; j >= 0; --j) {
      char c = record[j];

      if (c == '.' || c == '?') {
        dp[i][j] += dp[i][j + 1];
      }

      if (c == '#' || c == '?') {
        // can we match groupLen from j to j + groupLen - 1?
        bool matched = true;
        if (groupLen > record.size() - j)
          continue;

        for (int k = j; k < (int)(j + groupLen); ++k) {
          if (record[k] == '.') {
            matched = false;
            break;
          }
        }

        if ((uint32_t)j + groupLen < record.size() &&
            record[j + groupLen] == '#')
          matched = false;

        if (matched) { // && j + groupLen + 1 < numCol) {
          if (record[j + groupLen] == '.')
            dp[i][j] += dp[i - 1][j + groupLen];
          else if (j + groupLen + 1 < numCol)
            dp[i][j] += dp[i - 1][j + groupLen + 1];
        }
      }
    }
  }

  // dump table
#if 0
  cout << "    ";
  for (size_t i = 0; i < numCol; ++i)
    cout << "  " << i << " ";
  cout << "\n";

  cout << "    ";
  for (size_t i = 0; i < numCol; ++i)
    cout << "  " << record[i] << " ";
  cout << "\n";

  for (size_t i = 0; i < numRow; ++i) {
    cout << i << "   | ";
    for (size_t j = 0; j < numCol; ++j) {
      cout << dp[i][j] << " | ";
    }
    for (size_t k = 1; k <= i; ++k)
      cout << groups[k] << ", ";
    cout << "\n";
  }
  cout << "\n";
#endif
  return dp[numRow - 1][0];
}

vector<uint32_t> parseNums(const string &nstr) {
  vector<uint32_t> nums;
  bool hasnum = false;
  uint32_t cur = 0;
  for (uint32_t i = 0; i < nstr.length(); ++i) {
    if ((!isdigit(nstr[i]) && nstr[i] != '-') || (i == nstr.length() - 1)) {
      if (hasnum || isdigit(nstr[i])) {
        if (!isdigit(nstr[cur]) && nstr[cur] != '-')
          ++cur;
        uint32_t val = strtoll(nstr.c_str() + cur, nullptr, 10);
        nums.push_back(val);
        hasnum = false;
      }
      cur = i;
    } else {
      hasnum = true;
    }
  }
  return nums;
};

constexpr int NumExpansion = (5 - 1);
// constexpr int NumExpansion = (1 - 1);

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    string record = line.substr(0, line.find(' '));
    vector<uint32_t> baseD = parseNums(line.substr(line.find(' ')));

    string base = record;
    vector<uint32_t> damaged = baseD;
    for (int i = 0; i < NumExpansion; ++i) {
      record = record + "?" + base;
      damaged.insert(damaged.end(), baseD.begin(), baseD.end());
    }
    damaged.push_back(0);
    reverse(damaged.begin(), damaged.end());
    record += '.';

    sum += solve(record, damaged);
  }
  cout << "ans = " << sum << "\n";

  return 0;
}
