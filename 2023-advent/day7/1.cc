// https://adventofcode.com/2023/day/7

#include <bits/stdc++.h>
using namespace std;

/* format
32T3K 765
T55J5 684
KK677 28
KTJJT 220
QQQJA 483
*/

enum CARD_TYPES {
  INVALID = 0xff,
  ONE = 1,
  TWO,
  TWO_TWO,
  THREE,
  THREE_TWO,
  FOUR,
  FIVE,
};

struct Card {
  string card;
  uint32_t weight = 0;
  uint8_t type = INVALID;
  Card(const string &s, uint32_t w) : card(s), weight(w) {
    map<char, uint8_t> points;
    uint8_t numj = 0;
    for (char c : s) {
      if (c != 'J')
        points[c] += 1;
      else
        numj += 1;
    }

    if (numj) {
      uint8_t largerCnt = 0;
      char largerCntChar = 0;
      for (auto iter : points) {
        if (iter.second > largerCnt) {
          largerCnt = iter.second;
          largerCntChar = iter.first;
        }
      }
      points[largerCntChar] += numj;
    }

    if (points.size() == 1) {
      type = FIVE;
    } else if (points.size() == 2) {
      if (points.begin()->second == 1 || points.begin()->second == 4)
        type = FOUR;
      else
        type = THREE_TWO;
    } else if (points.size() == 5) {
      type = ONE;
    } else if (points.size() == 4) {
      type = TWO;
    } else {
      assert(points.size() == 3);
      type = THREE;
      for (auto iter : points) {
        if (iter.second == 2) {
          type = TWO_TWO;
          break;
        }
      }
    }
  }
  Card() = default;
};
vector<Card> Cards;
unordered_map<char, uint8_t> Strength = {
    {'A', 14}, {'K', 13}, {'Q', 12}, {'J', 1}, {'T', 10}, {'9', 9}, {'8', 8},
    {'7', 7},  {'6', 6},  {'5', 5},  {'4', 4}, {'3', 3},  {'2', 2}};

void solve() {
  sort(Cards.begin(), Cards.end(), [](const Card &a, const Card &b) {
    // A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, 2
    if (a.type != b.type)
      return a.type < b.type;

    for (size_t i = 0; i < 5; ++i)
      if (Strength[a.card.at(i)] != Strength[b.card.at(i)])
        return Strength[a.card.at(i)] < Strength[b.card.at(i)];

    return false;
  });
  uint64_t sum = 0;
  for (size_t i = 0; i < Cards.size(); ++i) {
    sum += Cards[i].weight * (i + 1);
  }

  cout << "ans = " << sum << "\n";
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

void getData(const string &s) {
  Cards.emplace_back(s.substr(0, 5), (uint32_t)parseNums(s.substr(5)).front());
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  for (std::string line; getline(input, line);) {
    getData(line);
  }
  solve();

  return 0;
}
