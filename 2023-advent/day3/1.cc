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
  uint32_t val = 0;
  Num() = default;
  Num(uint32_t s, uint32_t l, uint32_t v) : start(s), len(l), val(v) {}
};

struct Gear {
  uint32_t num[2] = {0, 0};
  uint8_t len = 0; // number of gears
  uint32_t start = 0;
  uint32_t row = 0;
  Gear(uint32_t s, uint32_t r) : start(s), row(r) {}
};

struct Line {
  vector<Num> nums;
  vector<Gear> gears;
  string str;
};

bool isValidWithCurLine(Gear &prevGear, const Line &curLine) {
  for (auto &numIter : curLine.nums) {
    //     *     (prevGear)
    // 1234 5678 (curLine)
    if ((numIter.start <= prevGear.start + 1 &&
         prevGear.start <= numIter.start + numIter.len)) {
      if (prevGear.len == 2) {
        cout << "invalid gear: " << prevGear.row << " " << prevGear.start
             << "\n";
        cout << "  cur/prev line: " << curLine.str << "\n";
        return false;
      }
      prevGear.num[prevGear.len] = numIter.val;
      prevGear.len += 1;
    }
  }
  return true;
}

bool isValidInCurLineOrPrevLine(Gear &curGear, const Line &prevLine,
                                const Line &curLine) {
  if (!isValidWithCurLine(curGear, prevLine))
    return false;

  if (!isValidWithCurLine(curGear, curLine))
    return false;

  return true;
}

const uint32_t InvalidPos = numeric_limits<uint32_t>::max();

uint32_t solve(const string &s, Line &prevLine, uint32_t rowNum) {
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
        curLine.nums.emplace_back(digStart, digLen,
                                  stoul(s.substr(digStart, digStart + digLen)));
        digStart = InvalidPos;
        digLen = 0;
      }
      if (s[cur] == '*') {
        curLine.gears.emplace_back(cur, rowNum);
      }
    }
    ++cur;
  }
  if (digStart != InvalidPos) {
    curLine.nums.emplace_back(digStart, digLen,
                              stoul(s.substr(digStart, digStart + digLen)));
    digStart = InvalidPos;
    digLen = 0;
  }

  uint32_t sum = 0;

  for (uint32_t i = prevLine.gears.size(); i > 0; --i) {
    if (!isValidWithCurLine(prevLine.gears[i - 1], curLine)) {
      prevLine.gears.erase(prevLine.gears.begin() + (i - 1));
    }
  }

  for (uint32_t i = curLine.gears.size(); i > 0; --i) {
    if (!isValidInCurLineOrPrevLine(curLine.gears[i - 1], prevLine, curLine)) {
      curLine.gears.erase(curLine.gears.begin() + (i - 1));
    }
  }

  for (auto &gearIter : prevLine.gears) {
    if (gearIter.len == 2)
      sum += gearIter.num[0] * gearIter.num[1];
  }

  prevLine.str = s;
  swap(prevLine.nums, curLine.nums);
  swap(prevLine.gears, curLine.gears);
  return sum;
}

int main(int argc, char *argv[]) {
  Line prevLine;
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  uint32_t row = 0;
  for (std::string line; getline(input, line);) {
    sum += solve(line, prevLine, row++);
  }
  string dummy;
  sum += solve(dummy, prevLine, row++);
  cout << sum << "\n";

  return 0;
}
