// https://adventofcode.com/2023/day/2

#include <bits/stdc++.h>
using namespace std;

// format: Game 1: 3 blue, 4 red; 1 red, 2 green, 6 blue; 2 green
// parse string
pair<bool, uint32_t> solve(const string &s) {
  size_t numStart = strlen("Game ");
  size_t numEnd = s.find_first_of(':', numStart);
  uint32_t id = stoul(s.substr(numStart, numEnd - numStart));

  uint32_t rgb[3] = {0, 0, 0};
  string tmpS = s.substr(numEnd);

  // matches: (smatch) num of red, num of green, num of blue
  regex numRgx("(\\d+) red|(\\d+) green|(\\d+) blue");
  smatch numMatch;

  while (regex_search(tmpS, numMatch, numRgx)) {
    uint32_t num = stoul(numMatch[0]);
    for (uint32_t i = 1; i <= 3; ++i) {
      if (numMatch[i].str().size()) {
        rgb[i - 1] = max(rgb[i - 1], (uint32_t)stoul(numMatch[i]));
        break;
      }
    }
    tmpS = numMatch.suffix().str();
  }
  cout << "max r, g, b = " << rgb[0] << ", " << rgb[1] << ", " << rgb[2]
       << "\n";

  uint32_t power = 1;
  for (int i = 0; i < 3; ++i)
    power *= rgb[i];
  return {true, power};
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  uint64_t sum = 0;
  for (std::string line; getline(input, line);) {
    auto r = solve(line);
    if (r.first)
      sum += r.second;
  }
  cout << sum << "\n";

  return 0;
}
