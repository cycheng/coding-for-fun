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
unordered_map<string, pair<string, string>> Graph;
uint64_t gcd(uint64_t a, uint64_t b) {
  if (b == 0)
    return a;
  return gcd(b, a % b);
}

// Function to return LCM of two numbers
uint64_t lcm(uint64_t a, uint64_t b) { return (a / gcd(a, b)) * b; }

void solve() {
  bool find = false;

  vector<string> cur;
  for (auto &iter : Graph) {
    if (iter.first.at(2) == 'A')
      cur.push_back(iter.first);
  }
  vector<uint32_t> numSteps;
  cout << "num start points : " << cur.size() << " \n";
  uint64_t cnt = 0;
  while (!find) {
    for (size_t i = 0; i < sequence.length(); ++i) {
      ++cnt;
      for (size_t j = 0; j < cur.size(); ++j) {
        auto iter = Graph.find(cur[j]);
        assert(iter != Graph.end());
        if (sequence[i] == 'L')
          cur[j] = iter->second.first;
        else
          cur[j] = iter->second.second;

        if (cur[j].at(2) == 'Z')
          numSteps.push_back(cnt);
      }
    }
    if (numSteps.size() == cur.size()) {
      find = true;
      break;
    }
  }
  uint64_t ans = numSteps[0];
  for (size_t i = 1; i < numSteps.size(); ++i) {
    cout << "lcm(" << ans << ", " << numSteps[i] << "\n";
    ans = lcm(ans, numSteps[i]);
  }

  cout << "ans = " << ans << "\n";
}

void getData(const string &s) {
  if (sequence.empty()) {
    sequence = s;
    return;
  }
  if (s.empty())
    return;

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
