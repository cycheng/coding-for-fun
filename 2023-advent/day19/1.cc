// https://adventofcode.com/2023/day/18

#include <bits/stdc++.h>
using namespace std;

// all parts begin in the workflow named in
//

struct Cond {
  char sym = 0;
  char cond = 0;
  int val = 0;
  string target;

  void print() { cout << sym << cond << val << ':' << target; }
};

struct Flow {
  vector<Cond> conds;
  string name;
  void print() {
    cout << name << ':' << '\n';
    for (auto &c : conds) {
      cout << "  ";
      c.print();
      cout << '\n';
    }
  }
};

enum {
  X = 0,
  M,
  A,
  S,
};

struct Rating {
  // int x = 0, m = 0, a = 0, s = 0;
  int val[4];
  void print() {
    for (int i = 0; i < 4; ++i)
      cout << val[i] << ' ';
    cout << '\n';
  }
};

Flow parseFlow(string s) {
  Flow flow;
  flow.name = s.substr(0, s.find('{'));
  s = s.substr(s.find('{') + 1);

  // a<2006:qkq,m>2090:A,rfg
  while (s.size()) {
    Cond cond;
    if (s.find(',') != string::npos) {
      string item = s.substr(0, s.find(','));
      s = s.substr(s.find(',') + 1);
      cond.sym = item[0];
      cond.cond = item[1];
      cond.val = stoi(item.substr(2, item.find(':')));
      cond.target = item.substr(item.find(':') + 1);
    } else {
      cond.target = s.substr(0, s.find('}'));
      s.clear();
    }
    flow.conds.push_back(cond);
  }
  return flow;
}

Rating parseRating(string s) {
  // {x=787,m=2655,a=1222,s=2876}
  Rating rate;
  for (int i = 0; i < 4; ++i) {
    size_t start = s.find('=') + 1, end = s.find(',');
    if (end == string::npos)
      end = s.find('}');
    rate.val[i] = stoi(s.substr(start, end - start));
    s.erase(s.begin(), s.begin() + end + 1);
  }
  return rate;
}

using Graph = unordered_map<string, Flow>;
using RatingList = vector<Rating>;

struct Range {
  int min[4] = {1, 1, 1, 1};
  int max[4] = {4000, 4000, 4000, 4000};
  Range() = default;
  // void copy(const Range &r) {
  //   for (int i = 0; i < 4; ++i) {
  //     min[i] = r.min[i];
  //     max[i] = r.max[i];
  //   }
  // }
  // Range(const Range &r) { copy(r); }
  // Range &operator=(const Range &r) {
  //   copy(r);
  //   return *this;
  // }
  uint64_t total() const {
    uint64_t s = 1;
    for (int i = 0; i < 4; ++i)
      if (min[i] < max[i])
        s *= max[i] - min[i] + 1;
      else
        s = 0;
    return s;
  }
  void print() const {
    cout << total() << ": ";
    for (int i = 0; i < 4; ++i) {
      cout << "[" << min[i] << " " << max[i] << "]";
    }
    cout << "\n";
  }
};

using RangeList = vector<Range>;

void splitRange(const string &target, const Graph &graph, const Range range,
                RangeList &accepted, vector<string> debug) {
  debug.push_back(target);
  const auto &iter = graph.find(target);
  if (target == "A") {
    accepted.push_back(range);
    range.print();
    for (auto &s : debug)
      cout << s << " -> ";
    cout << "\n\n";
    return;
  }
  if (target == "R")
    return;

  const Flow &flow = iter->second;
  Range newRange = range;
  for (const auto &cond : flow.conds) {
    int idx = -1;
    switch (cond.sym) {
    case 'x': idx = X; break;
    case 'm': idx = M; break;
    case 'a': idx = A; break;
    case 's': idx = S; break;
    case 0: break;
    default: assert(false);
    }

    switch (cond.cond) {
    case '<': {
      assert(idx >= 0);
      int origMax = newRange.max[idx];
      newRange.max[idx] = min(origMax, cond.val - 1);
      splitRange(cond.target, graph, newRange, accepted, debug);

      // apply the false condition.
      newRange.max[idx] = origMax;
      newRange.min[idx] = max(newRange.min[idx], cond.val);
      break;
    }

    case '>': {
      assert(idx >= 0);
      int origMin = newRange.min[idx];
      newRange.min[idx] = max(origMin, cond.val + 1);
      splitRange(cond.target, graph, newRange, accepted, debug);

      // apply the false condition.
      newRange.min[idx] = origMin;
      newRange.max[idx] = min(newRange.max[idx], cond.val);
      break;
    }

    case 0: {
      if (cond.target == "A")
        splitRange("A", graph, newRange, accepted, debug);
      else if (cond.target != "R")
        splitRange(cond.target, graph, newRange, accepted, debug);
      break;
    }

    default: assert(false);
    }
  }
}

uint64_t solve(Graph &graph, RatingList &rates) {
  uint64_t sum = 0;
  RangeList accepted;
  vector<string> debug;

  splitRange("in", graph, Range(), accepted, debug);
  for (auto &iter : accepted) {
    sum += iter.total();
  }
  cout << "accepted: " << accepted.size() << "\n";

  return sum;
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  unordered_map<string, Flow> flowMap;
  vector<Rating> rate;
  bool workflow = true;
  for (std::string line; getline(input, line);) {
    if (line.empty()) {
      workflow = false;
      continue;
    }
    if (workflow) {
      Flow flow = parseFlow(line);
      flowMap[flow.name] = flow;
    } else {
      rate.push_back(parseRating(line));
    }
  }
  cout << "ans = " << solve(flowMap, rate) << '\n';

#if 0
  for (auto &it : flowMap)
    it.second.print();
  for (auto &it : rate)
    it.print();
#endif
  return 0;
}
