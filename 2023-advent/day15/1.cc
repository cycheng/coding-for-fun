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

constexpr uint32_t InvalidLen = 0xffffffff;

struct Data {
  string label;
  uint32_t len = InvalidLen;
  uint32_t order = 0;

  Data(const string &s, uint32_t len) : label(s), len(len) {}
  Data() = default;
};

Data parseData(string &s) {
  Data d;
  size_t pos = s.find('=');
  if (pos != string::npos)
    d.len = stoul(s.substr(pos + 1));
  else
    pos = s.find('-');
  d.label = s.substr(0, pos);
  return d;
}

uint32_t solve(string &s) {
  size_t start = 0, end = 0;
  unordered_map<string, Data> boxes[256];
  uint32_t order = 0;
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
    Data d = parseData(sub);
    d.order = order++;
    uint32_t box = hashStr(d.label);
    auto iter = boxes[box].find(d.label);
    bool isRemove = d.len == InvalidLen;
    if (isRemove) {
      if (iter != boxes[box].end())
        boxes[box].erase(iter);
    } else {
      if (iter != boxes[box].end()) {
        iter->second.len = d.len;
      } else {
        boxes[box][d.label] = d;
      }
    }
    // cout << sub << " " << box << "\n";
  }

  uint32_t ans = 0;
  for (uint32_t i = 0; i < 256; ++i) {
    if (boxes[i].empty())
      continue;
    // order, len
    vector<pair<uint32_t, uint32_t>> oneBoxes;
    cout << "box " << i << "\n";
    for (auto &box : boxes[i])
      oneBoxes.emplace_back(box.second.order, box.second.len);

    sort(oneBoxes.begin(), oneBoxes.end(),
         [](const pair<uint32_t, uint32_t> &a,
            const pair<uint32_t, uint32_t> &b) { return a.first < b.first; });
    uint32_t sum = 0;
    for (uint32_t j = 0; j < oneBoxes.size(); ++j)
      sum += (i + 1) * (j + 1) * oneBoxes[j].second;

    cout << i << " " << sum << "\n";
    ans += sum;
  }
  return ans;
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
