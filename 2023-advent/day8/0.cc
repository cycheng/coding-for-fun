// https://adventofcode.com/2023/day/7

#include <bits/stdc++.h>
using namespace std;

/* format
RL

AAA = (BBB, CCC)
BBB = (DDD, EEE)
CCC = (ZZZ, GGG)
DDD = (DDD, DDD)
EEE = (EEE, EEE)
GGG = (GGG, GGG)
ZZZ = (ZZZ, ZZZ)
*/

string sequence;
string start, endStr;
unordered_map<string, pair<string, string>> Graph;

void solve() {
  start = "AAA", endStr = "ZZZ";
  bool find = false;
  string cur = start;
  cout << start << " -> " << endStr << "\n";
  uint32_t cnt = 0;
  while (!find) {
    for (size_t i = 0; i < sequence.length(); ++i) {
      ++cnt;
      auto iter = Graph.find(cur);
      assert(iter != Graph.end());
      if (sequence[i] == 'L')
        cur = iter->second.first;
      else
        cur = iter->second.second;

      if (cur == endStr) {
        find = true;
        break;
      }
    }
    if (find)
      break;
  }

  cout << "ans = " << cnt << "\n";
}

void getData(const string &s) {
  if (sequence.empty()) {
    sequence = s;
    return;
  }
  if (s.empty())
    return;

  // if (start.empty())
  //   start = s.substr(0, 3);
  // else
  //   endStr = s.substr(0, 3);

  Graph.insert({s.substr(0, 3), {s.substr(7, 3), s.substr(12, 3)}});
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    getData(line);
  }
  solve();

  return 0;
}
