// https://adventofcode.com/2023/day/1

#include <bits/stdc++.h>
using namespace std;

const unordered_map<string, uint8_t> validStr = {
    {"0", 0},   {"1", 1},     {"2", 2},     {"3", 3},    {"4", 4},
    {"5", 5},   {"6", 6},     {"7", 7},     {"8", 8},    {"9", 9},
    {"one", 1}, {"two", 2},   {"three", 3}, {"four", 4}, {"five", 5},
    {"six", 6}, {"seven", 7}, {"eight", 8}, {"nine", 9}};
uint8_t minLen = 255, maxLen = 0;

pair<bool, uint8_t> isdigit(const string &s, size_t start) {
  for (uint32_t i = minLen; i <= maxLen; ++i) {
    string t = s.substr(start, i);
    auto iter = validStr.find(t);
    if (iter != validStr.end())
      return {true, iter->second};
  }
  return {false, -1};
}
// one, two, three, four, five, six, seven, eight, nine
uint8_t solve(const string &s) {
  uint8_t sum = 0;
  size_t i;

  for (i = 0; i < s.length(); ++i) {
    auto r = isdigit(s, i);
    if (r.first) {
      sum = r.second * 10;
      break;
    }
  }
  assert(i < s.length());
  for (i = s.length(); i > 0; --i) {
    auto r = isdigit(s, i - 1);
    if (r.first) {
      sum += r.second;
      break;
    }
  }
  assert(i > 0);
  return sum;
}

int main(int argc, char *argv[]) {
  for (auto &vstr : validStr) {
    minLen = min(minLen, (uint8_t)vstr.first.length());
    maxLen = max(maxLen, (uint8_t)vstr.first.length());
  }

  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    sum += solve(line);
  }
  cout << sum << "\n";

  return 0;
}
