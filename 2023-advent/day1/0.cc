// https://adventofcode.com/2023/day/1

#include <bits/stdc++.h>
using namespace std;

uint8_t solve(const string &s) {
  uint64_t sum = 0;
  size_t i;
  for (i = 0; i < s.length(); ++i)
    if (isdigit(s[i]))
      break;
  assert(i < s.length());
  sum = (uint8_t)(s[i] - '0') * 10;
  for (i = s.length(); i > 0; --i)
    if (isdigit(s[i - 1]))
      break;
  assert(i > 0);
  sum += s[i - 1] - '0';
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
