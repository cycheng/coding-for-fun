// https://adventofcode.com/2023/day/3

#include <bits/stdc++.h>
using namespace std;

/* format
seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4
*/

struct Map {
  uint64_t src = 0, dst = 0;
  int64_t len = 0;
  Map() = default;
  Map(uint64_t s, uint64_t d, int64_t l) : src(s), dst(d), len(l) {}
};

// src id -> Mapping info
vector<map<uint64_t, Map>> Src2Dst;
vector<uint64_t> Seeds;

bool isOverlaped(uint64_t key, const Map &mapping) {
  if (mapping.len > 0) {
    if (key < mapping.src || key >= mapping.src + mapping.len)
      return false;
  } else {
    if (key > mapping.src || (int64_t)key <= (int64_t)mapping.src + mapping.len)
      return false;
  }
  return true;
}

uint64_t getMappedValue(uint64_t key, const Map &mapping) {
  assert(isOverlaped(key, mapping));
  if (mapping.len > 0)
    return (key - mapping.src) + mapping.dst;
  return mapping.dst - (mapping.src - key);
}

uint64_t getSeedMappedResult(uint64_t key) {
  uint64_t curSrc = key;

  // cout << curSrc << " -> ";
  for (auto &mapping : Src2Dst) {
    auto iter = mapping.lower_bound(curSrc);
    if (iter != mapping.end()) {
      if (isOverlaped(curSrc, iter->second)) {
        curSrc = getMappedValue(curSrc, iter->second);
      } else {
        // curSrc -> curSrc : unchanged.
      }
    } else {
      // curSrc -> curSrc : unchanged.
    }
    // cout << curSrc << " -> ";
  }
  // cout << curSrc << "\n";
  return curSrc;
}

void solve() {
  // cout << "------------------------\n";
  uint64_t nearest = numeric_limits<uint64_t>::max();
  for (size_t i = 0; i < Seeds.size(); i += 2) {
    uint64_t start = Seeds[i], end = start + Seeds[i + 1];
    for (; start != end; ++start)
      nearest = min(nearest, getSeedMappedResult(start));
  }
  cout << "ans: " << nearest << "\n";
}

void genMap(const string &s) {
  auto parsenums = [](const string &nstr) -> vector<uint64_t> {
    vector<uint64_t> nums;
    bool hasnum = false;
    uint32_t cur = 0;
    for (uint32_t i = 0; i < nstr.length(); ++i) {
      if (!isdigit(nstr[i]) || (i == nstr.length() - 1)) {
        if (hasnum || isdigit(nstr[i])) {
          uint64_t val = strtoull(nstr.c_str() + cur, nullptr, 10);
          nums.push_back(val);
          hasnum = false;
          cur = i;
        }
      } else {
        hasnum = true;
      }
    }
    return nums;
  };

  if (s.find("map") != std::string::npos) {
    Src2Dst.push_back({});
    if (Src2Dst.size() > 1) {
      // cout << "1 ===================\n";
      int printIdx = Src2Dst.size() - 2;
      for (auto &iter : Src2Dst[printIdx]) {
        (void)iter;
        // cout << iter.first << ": " << iter.second.src << " -> ";
        // cout << iter.second.dst << " (" << iter.second.len << ")\n";
      }
    }
    return;
  }

  if (s.find("seeds:") != std::string::npos) {
    Seeds = parsenums(s.substr(6));
    // cout << "2 ===================\n";
    for (uint64_t v : Seeds) {
      (void)v;
      // cout << v << " ";
    }
    // cout << "\n";
    return;
  }

  if (s.empty())
    return;

  auto nums = parsenums(s);
  assert(nums.size() == 3);

  uint64_t src = nums[1], dst = nums[0], len = nums[2];

  assert(len <= numeric_limits<int64_t>::max());

  Src2Dst.back().insert({src, {src, dst, (int64_t)len}});
  if (len) {
    src = src + len - 1;
    dst = dst + len - 1;
    int64_t newlen = -(int64_t)len;
    Src2Dst.back().insert({src, {src, dst, newlen}});
  }
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    genMap(line);
  }
  solve();
  cout << "correct answer for 0-in-test.txt is " << 46 << "\n";
  cout << "correct answer for 0-in.txt is " << 69841803 << "\n";

  return 0;
}
