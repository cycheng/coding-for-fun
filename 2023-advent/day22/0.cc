// https://adventofcode.com/2023/day/22

#include <bits/stdc++.h>
using namespace std;

enum { X = 0, Y, Z };
struct Pos {
  // x, y, z
  int start[3] = {0, 0, 0};
  int end[3] = {0, 0, 0};
  void print() {
    for (int i = 0; i < 3; ++i)
      cout << start[i] << " ";
    cout << "~ ";
    for (int i = 0; i < 3; ++i)
      cout << end[i] << " ";
    // cout << " - " << name << "\n";
    cout << "\n";
    // assert(start[X] == end[X] || start[Y] == end[Y]);
  }
  string name;
  int id = 0;
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
  sort(plist.begin(), plist.end(),
       [](const Pos &a, const Pos &b) { return a.start[Z] < b.start[Z]; });
  PosList rearranged;
  rearranged.reserve(plist.size());
  unordered_set<int> disintegrateSet;
  // unordered_set<int> topSet;
  //  for (int i = 0; i < (int)plist.size(); ++i)
  //    topSet.insert(plist[i].id);

  for (int i = 0; i < (int)plist.size(); ++i) {
    Pos p = plist[i];
    int minZ = 1;
    // <Z, set of conflict in that Z>
    // unordered_map<int, unordered_set<int>> conflictZ;
    for (int j = 0; j < (int)rearranged.size(); ++j) {
      if (intersectXY(p, rearranged[j])) {
        minZ = max(minZ, rearranged[j].end[Z] + 1);
        // conflictZ[rearranged[j].end[Z] + 1].insert(j);
        // topSet.erase(j);
      }
    }
    assert(minZ <= p.start[Z]);
    int zdiff = p.start[Z] - minZ;
    p.end[Z] -= zdiff;
    p.start[Z] = minZ;
    rearranged.push_back(p);
    // for (const auto &iter : conflictZ) {
    //   if (iter.second.size() > 1 && iter.first == minZ) {
    //     conflictSet.insert(iter.second.begin(), iter.second.end());
    //     // cout << p.name << " conflict: ";
    //     //  for (auto id : iter.second)
    //     //    cout << rearranged[id].name << " ";
    //     // cout << "\n";
    //   }
    // }
  }
  sort(rearranged.begin(), rearranged.end(),
       [](const Pos &a, const Pos &b) { return a.start[Z] < b.start[Z]; });

  assert(rearranged.size() == plist.size());
  // vector<Support> suooprtList;
  unordered_map<int, Support> supportMap;
  supportMap.reserve(rearranged.size());
  // suooprtList.reserve(rearranged.size());
  // for (int i = 0; i < (int)rearranged.size(); ++i)
  //   suooprtList.emplace_back(rearranged[i].id);

  for (int i = 0; i < (int)rearranged.size(); ++i) {
    const Pos &pi = rearranged[i];
    // unordered_set<int> conflictSet;
    //  assert(pi.start[Z] >= rearranged[i - 1].start[Z]);
    // Support &supi = suooprtList[i];
    Support &supi = supportMap[pi.id];
    supi.id = pi.id;
    // assert(supi.id == pi.id);
    for (int j = 0; j < (int)rearranged.size(); ++j) {
      if (i == j)
        continue;
      const Pos &pj = rearranged[j];
      Support &supj = supportMap[pj.id];
      supj.id = pj.id;
      // Support &supj = suooprtList[j];
      // assert(supj.id == pj.id);
      if ((pj.end[Z] + 1 == pi.start[Z]) && intersectXY(pi, pj)) {
        // conflictSet.insert(j);
        supj.supports.insert(supi.id);
        supi.supportedBy.insert(supj.id);
      }
    }
    // if (conflictSet.size() > 1) {
    //   disintegrateSet.insert(conflictSet.begin(), conflictSet.end());
    // }
  }

  int ans = 0;
  // for (auto &supi : suooprtList) {
  for (auto &supIter : supportMap) {
    int supi = supIter.first;
    bool canDisintegrate{true};
    // supi.print(supi.id);
    bool dbgPrint = false;
    Pos &dbgp = BrickMap[supi];
    if (dbgp.start[X] == 1 && dbgp.start[Y] == 6 && dbgp.start[Z] == 246) {
      cout << "here\n";
      dbgPrint = true;
      dbgp.print();
    }

    if (dbgp.start[X] == 2 && dbgp.start[Y] == 2 && dbgp.start[Z] == 247) {
      cout << "here\n";
      dbgPrint = true;
      dbgp.print();
    }

    for (auto &supj : supIter.second.supports) {
      if (dbgPrint) {
        // suooprtList[supj].print(supj);
        supportMap[supj].print(supj);
      }
      // if (suooprtList[supj].supportedBy.size() < 2) {
      if (supportMap[supj].supportedBy.size() < 2) {
        canDisintegrate = false;
        break;
      }
    }
    // if (canDisintegrate)
    //   supi.print(supi.id);
    ans += canDisintegrate;
  }

  // for (auto &iter : rearranged)
  //   iter.print();

  cout << "ans = " << ans << "\n";
  // disintegrateSet.insert(topSet.begin(), topSet.end());
  //  cout << disintegrateSet.size() << "\n";
  //  cout << topSet.size() << "\n";

  // cout << "ans = " << disintegrateSet.size() << "\n";
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  PosList plist;
  int id = 0;
  for (std::string line; getline(input, line);) {
    parse(plist, line, id++);
  }
  solve(plist);
  //   for (auto &iter : plist)
  //     iter.print();
  return 0;
}