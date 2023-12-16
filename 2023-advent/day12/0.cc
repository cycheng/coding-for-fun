// https://adventofcode.com/2023/day/12

#include <bits/stdc++.h>
using namespace std;

/* format
???.### 1,1,3
*/

uint32_t bruteforce(string &record, vector<uint32_t> &damaged, uint32_t curR,
                    uint32_t curD, uint32_t dCnt) {
  uint32_t sum = 0;
  for (size_t i = curR; i < record.length(); ++i) {
    char c = record[i];
    if (c == '#') {
      if (dCnt == 0) {
        curD++;
      }
      ++dCnt;
      if (curD >= damaged.size() || dCnt > damaged[curD])
        return sum;
    } else if (c == '.') {
      // reset dCnt.
      if (dCnt != damaged[curD] && (i > 0 && record[i - 1] == '#'))
        return sum;
      dCnt = 0;
    } else if (c == '?') {
      // assume '.'
      record[i] = '.';
      sum += bruteforce(record, damaged, i, curD, dCnt);

      // assume '#'
      record[i] = '#';
      sum += bruteforce(record, damaged, i, curD, dCnt);

      record[i] = '?';
      return sum;
    }
  }

  if (curD != damaged.size() - 1 || (dCnt != 0 && dCnt != damaged[curD]))
    return sum;

  return sum + 1;
}

uint32_t solve(string &record, vector<uint32_t> damaged) {
  uint32_t r = bruteforce(record, damaged, 0, 0, 0);
  cout << r << "\n";
  return r;
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

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    string record = line.substr(0, line.find(' '));
    vector<uint32_t> damaged = parseNums(line.substr(line.find(' ')));
    // insert a dummy zero.
    damaged.insert(damaged.begin(), 0);
    cout << line.substr(line.find(' ')) << "\n";
    cout << record << "\n\n";

    sum += solve(record, damaged);
  }
  cout << "ans = " << sum << "\n";

  return 0;
}
