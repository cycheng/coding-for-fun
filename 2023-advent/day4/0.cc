// https://adventofcode.com/2023/day/3

#include <bits/stdc++.h>
using namespace std;

/* format
Card 1: 41 48 83 86 17 | 83 86  6 31 17  9 48 53
*/

uint32_t solve(const string &s) {
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
  uint32_t sum = 0;
  for (auto &niter : numset) {
    if (winset.count(niter)) {
      if (sum == 0) {
        sum = 1;
      } else {
        sum *= 2;
      }
    }
  }
  return sum;
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    sum += solve(line);
  }
  cout << sum << "\n";

  return 0;
}
