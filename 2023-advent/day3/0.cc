// https://adventofcode.com/2023/day/3

#include <bits/stdc++.h>
using namespace std;

/* format
    ...*...... (prev)
    ..35..633. (cur)
    ......#...
*/

struct Num {
  uint32_t start = 0;
  uint32_t len = 0;
  Num() = default;
  Num(uint32_t s, uint32_t l) : start(s), len(l) {}
};

struct Line {
  vector<Num> nums;
  string str;
};

const uint32_t InvalidPos = numeric_limits<uint32_t>::max();

bool isValidInCurLineOrPrevLine(const Num &num, const string &prevLine,
                                const string &curLine) {
  uint32_t i = num.start;
  if (i > 0)
    --i;

  if ((!isdigit(curLine[i]) && curLine[i] != '.') ||
      (num.start + num.len < curLine.length() &&
       curLine[num.start + num.len] != '.'))
    return true;

  for (; i <= num.start + num.len; ++i)
    if (i < prevLine.length()) {
      char c = prevLine[i];
      if (!isdigit(c) && c != '.')
        return true;
    }
  return false;
}

bool isValidWithCurLine(const Num &prevNum, const string &curLine) {
  uint32_t i = prevNum.start;
  if (i > 0)
    --i;

  for (; i <= prevNum.start + prevNum.len; ++i)
    if (i < curLine.length()) {
      char c = curLine[i];
      if (!isdigit(c) && c != '.')
        return true;
    }
  return false;
}

uint32_t solve(const string &s, Line &prevLine) {
  size_t cur = 0;
  uint32_t digStart = InvalidPos, digLen = 0;
  Line curLine;

  curLine.str = s;
  while (cur < s.length()) {
    if (isdigit(s[cur])) {
      if (digStart == InvalidPos) {
        digStart = cur;
        digLen = 1;
      } else {
        ++digLen;
      }
    } else {
      if (digStart != InvalidPos) {
        curLine.nums.emplace_back(digStart, digLen);
        digStart = InvalidPos;
        digLen = 0;
      }
    }
    ++cur;
  }
  if (digStart != InvalidPos) {
    curLine.nums.emplace_back(digStart, digLen);
    digStart = InvalidPos;
    digLen = 0;
  }

  uint32_t sum = 0;

  for (uint32_t i = prevLine.nums.size(); i > 0; --i) {
    if (isValidWithCurLine(prevLine.nums[i - 1], s)) {
      string part = prevLine.str.substr(prevLine.nums[i - 1].start,
                                        prevLine.nums[i - 1].len);
      cout << "++ find part: " << part << "\n";
      sum += stoul(part);
      prevLine.nums.erase(prevLine.nums.begin() + (i - 1));
    }
  }

  for (uint32_t i = curLine.nums.size(); i > 0; --i) {
    if (isValidInCurLineOrPrevLine(curLine.nums[i - 1], prevLine.str, s)) {
      string part = curLine.str.substr(curLine.nums[i - 1].start,
                                       curLine.nums[i - 1].len);
      sum += stoul(part);

      cout << "-- find part: " << part << "\n";
      curLine.nums.erase(curLine.nums.begin() + (i - 1));
    }
  }
  prevLine.str = s;
  swap(prevLine.nums, curLine.nums);
  return sum;
}

int main(int argc, char *argv[]) {
  Line prevLine;
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    sum += solve(line, prevLine);
  }
  cout << sum << "\n";

  return 0;
}
