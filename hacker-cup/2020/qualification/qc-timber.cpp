// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/C
//
//
// Each tree may be cut down such that it falls either to the left or to the
// right, or it may be left standing (it's important to farm trees sustainably).
// If the iith tree is cut down to the left, it will form a timber interval of
// [Pi - Hi, Pi], on the ground, while if it is cut down to the right, it will
// form a timber interval [Pi, Pi + Hi]

#include <bits/stdc++.h>
using namespace std;

struct tree {
  int pos = 0, height = 0;
  tree(int p, int h) : pos(p), height(h) {}
  tree() = default;
};

int solve() {
  // <pos, height>
  int n;
  cin >> n;
  vector<tree> trees(n);
  for (int i = 0; i < n; ++i) {
    int p, h;
    cin >> p >> h;
    trees.emplace_back(p, h);
  }
  sort(trees.begin(), trees.end(),
       [](const tree &a, const tree &b) { return a.pos < b.pos; });

  // <x, max length>: x as the end point, its maximum length
  map<int, int> dp;
  // unordered_map<int, int> dp;
  // dp.reserve(n);
  for (const auto &tree : trees) {
    int leftlen = dp[tree.pos - tree.height];
    int rightlen = dp[tree.pos];

    dp[tree.pos] = max(rightlen, leftlen + tree.height);
    dp[tree.pos + tree.height] =
        max(dp[tree.pos + tree.height], rightlen + tree.height);
  }

  int ans = 0;
  for (const auto &d : dp)
    ans = max(ans, d.second);
  return ans;
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    string s;
    cout << "Case #" << i << ": " << solve();
    cout << endl;
  }
  return 0;
}