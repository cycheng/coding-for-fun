// https://adventofcode.com/2023/day/22

#include <bits/stdc++.h>
using namespace std;

enum { X = 0, Y, Z };
struct Pos {
  // x, y, z
  int start[3] = {0, 0, 0};
  int end[3] = {0, 0, 0};
  int id = 0;
  void print() {
    for (int i = 0; i < 3; ++i)
      cout << start[i] << " ";
    cout << "~ ";
    for (int i = 0; i < 3; ++i)
      cout << end[i] << " ";
    cout << "\n";
  }
};

std::unordered_map<int, Pos> BrickMap;

using PosList = vector<Pos>;

// format: 1,1,8~1,1,9
void parse(PosList &plist, string &line, int id) {
  stringstream sstrm(line);

  Pos p;
  for (int i = 0; i < 3; ++i) {
    char tmp;
    sstrm >> p.start[i];
    sstrm >> tmp;
  }

  for (int i = 0; i < 3; ++i) {
    char tmp;
    sstrm >> p.end[i];
    sstrm >> tmp;
  }
  p.id = id;
  BrickMap[id] = p;
  plist.emplace_back(p);
}

bool intersectXY(const Pos &a, const Pos &b) {
  if (b.end[X] < a.start[X] || a.end[X] < b.start[X])
    return false;
  if (b.end[Y] < a.start[Y] || a.end[Y] < b.start[Y])
    return false;
  return true;
}

struct Support {
  int id;
  std::unordered_set<int> supports;
  std::unordered_set<int> supportedBy;

  Support() = default;
  Support(int id) : id(id) {}

  void print(int myid) {
    assert(myid == id);
    BrickMap[myid].print();
    cout << "  supports:\n";
    for (auto &iter : supports) {
      cout << "    ";
      BrickMap[iter].print();
    }
    cout << "  supportedBy:\n";
    for (auto &iter : supportedBy) {
      cout << "    ";
      BrickMap[iter].print();
    }
  }
};

void solve(PosList &plist) {
  // sort by starting Z pos.
  sort(plist.begin(), plist.end(),
       [](const Pos &a, const Pos &b) { return a.start[Z] < b.start[Z]; });
  PosList rearranged;
  rearranged.reserve(plist.size());

  // place the Position started from the small Z pos.
  for (int i = 0; i < (int)plist.size(); ++i) {
    Pos p = plist[i];
    int minZ = 1;
    for (int j = 0; j < (int)rearranged.size(); ++j)
      if (intersectXY(p, rearranged[j]))
        minZ = max(minZ, rearranged[j].end[Z] + 1);

    int zdiff = p.start[Z] - minZ;
    p.end[Z] -= zdiff;
    p.start[Z] = minZ;
    rearranged.push_back(p);
  }
  sort(rearranged.begin(), rearranged.end(),
       [](const Pos &a, const Pos &b) { return a.start[Z] < b.start[Z]; });

  unordered_map<int, Support> supportMap;
  supportMap.reserve(rearranged.size());
  for (const Pos &pos : rearranged)
    supportMap[pos.id].id = pos.id;

  for (int i = 0; i < (int)rearranged.size(); ++i) {
    const Pos &pi = rearranged[i];
    Support &supi = supportMap[pi.id];
    for (int j = 0; j < (int)rearranged.size(); ++j) {
      if (i == j)
        continue;
      const Pos &pj = rearranged[j];
      Support &supj = supportMap[pj.id];
      if ((pj.end[Z] + 1 == pi.start[Z]) && intersectXY(pi, pj)) {
        supj.supports.insert(supi.id);
        supi.supportedBy.insert(supj.id);
      }
    }
  }

  int ans = 0;
  for (auto &supIter : supportMap) {
    bool canDisintegrate = true;
    for (auto &supj : supIter.second.supports) {
      if (supportMap[supj].supportedBy.size() < 2) {
        canDisintegrate = false;
        break;
      }
    }
    ans += canDisintegrate;
  }

  cout << "ans = " << ans << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  PosList plist;
  int id = 0;
  for (std::string line; getline(input, line);) {
    parse(plist, line, id++);
  }
  solve(plist);
  return 0;
}