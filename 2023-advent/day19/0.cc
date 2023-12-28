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

string getNext(Rating &rate, Graph &graph, string &src) {
  const auto &iter = graph.find(src);
  int val;

  for (auto &cond : iter->second.conds) {
    if (cond.sym) {
      switch (cond.sym) {
      case 'x': val = rate.val[X]; break;
      case 'm': val = rate.val[M]; break;
      case 'a': val = rate.val[A]; break;
      case 's': val = rate.val[S]; break;
      default: assert(false);
      }
    }

    switch (cond.cond) {
    case '<':
      if (val < cond.val)
        return cond.target;
      break;
    case '>':
      if (val > cond.val)
        return cond.target;
      break;
    case 0: break;
    default: assert(false);
    }
  }
  return iter->second.conds.back().target;
}

uint32_t solve(Graph &graph, RatingList &rates) {
  uint32_t sum = 0;
  for (auto &rate : rates) {
    cout << "check: ";
    rate.print();

    string start = "in";
    while (start != "R" && start != "A") {
      cout << start << " -> ";
      start = getNext(rate, graph, start);
    }
    cout << start << "\n";
    if (start == "A") {
      for (int i = 0; i < 4; ++i)
        sum += rate.val[i];
    }
  }
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
