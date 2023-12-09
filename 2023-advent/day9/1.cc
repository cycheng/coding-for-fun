// https://adventofcode.com/2023/day/7

#include <bits/stdc++.h>
using namespace std;

/* format
10  13  16  21  30  45  68
*/

vector<int64_t> seq;
vector<int64_t> parseNums(const string &nstr) {
  vector<int64_t> nums;
  bool hasnum = false;
  uint32_t cur = 0;
  for (uint32_t i = 0; i < nstr.length(); ++i) {
    if ((!isdigit(nstr[i]) && nstr[i] != '-') || (i == nstr.length() - 1)) {
      if (hasnum || isdigit(nstr[i])) {
        int64_t val = strtoll(nstr.c_str() + cur, nullptr, 10);
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

bool allZeros(vector<int64_t> &sequence) {
  for (int64_t v : sequence)
    if (v)
      return false;
  return true;
}

int64_t computeFirstValue(vector<int64_t> &sequence) {
  vector<int64_t> nextseq;
  size_t len = sequence.size();
  assert(len >= 2);
  if (!allZeros(sequence)) {
    for (size_t i = 1; i < sequence.size(); ++i)
      nextseq.push_back(sequence[i] - sequence[i - 1]);
    return sequence.front() - computeFirstValue(nextseq);
  }
  return 0;
}

int64_t findNum(size_t index, size_t height) { return computeFirstValue(seq); }

int64_t solve(const string &s) {
  seq = parseNums(s);
  return findNum(seq.size(), 0);
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  int64_t sum = 0;
  for (std::string line; getline(input, line);) {
    int64_t num = solve(line);
    cout << num << "\n";
    sum += num;
  }
  cout << "ans = " << sum << "\n";
  cout << "Correct answer for the 0-in.txt: " << 1068 << "\n";
  return 0;
}
