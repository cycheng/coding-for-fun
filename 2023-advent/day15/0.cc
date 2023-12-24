// https://adventofcode.com/2023/day/15

#include <bits/stdc++.h>
using namespace std;

/* format
rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7
*/

uint16_t hashStr(string &s) {
  uint16_t v = 0;
  for (char c : s) {
    v += (uint16_t)c;
    v *= 17;
    v %= 256;
  }
  return v;
}

uint32_t solve(string &s) {
  size_t start = 0, end = 0;
  uint32_t sum = 0;
  while (start != string::npos) {
    end = s.find(',', start);
    string sub;
    if (end != string::npos) {
      sub = s.substr(start, end - start);
      start = end + 1;
    } else {
      sub = s.substr(start);
      start = end;
    }
    sum += hashStr(sub);
    cout << sub << " " << hashStr(sub) << "\n";
  }
  return sum;
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    sum += solve(line);
  }
  cout << "ans = " << sum << "\n";

  return 0;
}
