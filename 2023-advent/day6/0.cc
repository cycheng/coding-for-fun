// https://adventofcode.com/2023/day/6

#include <bits/stdc++.h>
using namespace std;

/* format
Time:      7  15   30
Distance:  9  40  200
*/

vector<vector<uint64_t>> Data;

void solve() {
  uint64_t total = 1;
  for (size_t i = 0; i < Data[0].size(); ++i) {
    // step = hold_time
    // dist = step * (Time - hold_time) = step*Time - step*hold_time > Distance
    uint64_t time = Data[0][i], distance = Data[1][i];
    cout << "time: " << time << ", distance: " << distance << ": ";

    uint64_t sum = 0;
    for (uint64_t step = 1; step < time; ++step) {
      uint64_t dist = step * (time - step);
      if (dist > distance)
        ++sum;
    }
    cout << sum << "\n";
    total *= sum;
  }
  cout << "ans = " << total << "\n";
}

vector<uint64_t> parseNums(const string &nstr) {
  vector<uint64_t> nums;
  bool hasnum = false;
  uint32_t cur = 0;
  for (uint32_t i = 0; i < nstr.length(); ++i) {
    if (!isdigit(nstr[i]) || (i == nstr.length() - 1)) {
      if (hasnum || isdigit(nstr[i])) {
        uint64_t val = strtoull(nstr.c_str() + cur, nullptr, 10);
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

void getData(const string &s) { Data.push_back(parseNums(s)); }

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    getData(line);
  }
  solve();

  return 0;
}
