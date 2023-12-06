// https://adventofcode.com/2023/day/5

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

struct Range {
  uint64_t dst = 0;
  uint64_t len = 0;
  Range() = default;
  Range(uint64_t d, int64_t l) : dst(d), len(l) {}
};

// map: ranges (source -> Range (dest, len))
vector<map<uint64_t, Range>> RangesList;
vector<Range> Seeds;

void printRanges(vector<Range> &ranges) {
  for (auto &r : ranges)
    cout << r.dst << " " << r.len << "\n";
}

void printRanges(map<uint64_t, Range> &ranges) {
  for (auto &range : ranges) {
    cout << range.first << " -> " << range.second.dst << " ("
         << range.second.len << ")\n";
  }
  cout << "\n";
}

void solve() {
  for (auto &ranges : RangesList) {
    vector<Range> NewSeeds;
    NewSeeds.reserve(Seeds.size());
    for (size_t i = 0; i < Seeds.size(); ++i) {
      uint64_t start = Seeds[i].dst;
      uint64_t len = Seeds[i].len;
      auto range = ranges.lower_bound(start);
      //              case 1:      case 2:          case 3:       case 4:
      // seed:          +----+     +---+         +---------+       +---+
      // range:       +-----+        +-------+      +---+      +-+
      //
      // => new seed:   +---++     +-+-+         +--+---+--+   unchanged
      //
      // add new splitted end-seed to the end of Seeds to continue the check.
      // add unchanged seeds and mapped seeds to the NewSeeds.

      // seed     +----+
      // range  +--+  +---+

      if (range != ranges.begin()) {

        auto prevrange = prev(range);
        if (start < (prevrange->first + prevrange->second.len))
          range = prevrange;
      }

      if (range == ranges.end())
        range = prev(range);

      uint64_t rngstart = range->first;
      uint64_t dst = range->second.dst;
      uint64_t rnglen = range->second.len;
      // case 4: range is ranges.end() at init
      if (rngstart + rnglen <= start) {
        // direct map
        NewSeeds.emplace_back(start, len);
        continue;
      }

      // case 1: range is ranges.end() at init
      if (rngstart <= start && start < rngstart + rnglen) {
        uint64_t newlen = min((rngstart + rnglen) - start, len);
        uint64_t newstart = dst + (start - rngstart);
        NewSeeds.emplace_back(newstart, newlen);

        if (newlen < len) {
          Seeds.emplace_back(start + newlen, len - newlen);
        }
        continue;
      }

      // range is not ranges.end() at init
      if (start + len <= rngstart) {
        // direct map
        NewSeeds.emplace_back(start, len);
        continue;
      }

      //              case 1:      case 2:          case 3:       case 4:
      // seed:          +----+     +---+         +---------+       +---+
      // range:       +-----+        +-------+      +---+      +-+
      //
      // => new seed:   +---++     +-+-+         +--+---+--+   unchanged
      {
        if (start < rngstart) {
          // split and do direct map
          NewSeeds.emplace_back(start, rngstart - start);
        }
        len = len - (rngstart - start);
        start = rngstart;

        // map middle part:
        uint64_t newlen = min((rngstart + rnglen) - start, len);
        uint64_t newstart = dst + (rngstart - start);
        NewSeeds.emplace_back(newstart, newlen);

        // add end part to Seeds.
        if (newlen < len) {
          Seeds.emplace_back(start + newlen, len - newlen);
        }
      }
    }
    swap(Seeds, NewSeeds);
    cout << "---- map ----\n";
    printRanges(ranges);
    cout << "---- transformed ----\n";
    printRanges(Seeds);
    cout << "\n";
  }
  sort(Seeds.begin(), Seeds.end(),
       [](const Range &a, const Range &b) { return a.dst < b.dst; });
  cout << "num ranges in the end: " << Seeds.size() << "\n";
  cout << "ans = " << Seeds.front().dst << "\n";
}

vector<uint64_t> parseNums(const string &nstr) {
  vector<uint64_t> nums;
  bool hasnum = false;
  uint32_t cur = 0;
  for (uint32_t i = 0; i < nstr.length(); ++i) {
    if (!isdigit(nstr[i]) || (i == nstr.length() - 1)) {
      if (hasnum || isdigit(nstr[i])) {
        uint64_t val = strtoull(nstr.c_str() + cur, nullptr, 10);
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

void genMap(const string &s) {
  if (s.find("map") != std::string::npos) {
    RangesList.push_back({});
    return;
  }

  if (s.find("seeds:") != std::string::npos) {
    auto nums = parseNums(s);
    for (size_t i = 0; i < nums.size(); i += 2) {
      Seeds.emplace_back(nums[i], nums[i + 1]);
    }
    cout << "-------- seeds \n";
    for (auto v : Seeds) {
      (void)v;
      cout << v.dst << " " << v.len << "\n";
    }
    cout << "\n";
    return;
  }

  if (s.empty())
    return;

  auto nums = parseNums(s);
  assert(nums.size() == 3);

  uint64_t src = nums[1], dst = nums[0], len = nums[2];

  assert(len <= numeric_limits<int64_t>::max());

  RangesList.back().emplace(src, Range(dst, len));
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
