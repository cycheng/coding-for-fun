// https://www.facebook.com/codingcompetitions/hacker-cup/2020/qualification-round/problems/B
// #greedy, #two pointer, #array, #like, #15
#include <bits/stdc++.h>
using namespace std;

// greedy
bool solve(string &s) {
  if (s.size() == 1)
    return true;
  const int last = s.length() - 1;
  int left = last, right = last;
  char colors[3];
  while (left >= 0) {
    colors[right - left] = s[left];
    if (right - left == 2) {
      // めぐみん - エクスプロージョン!! >w<
      if (colors[0] == colors[1] && colors[1] == colors[2]) {
        // drop the last element.
        --right;
      }

      // erase two elements from s[left+1]
      if (right - left == 2) {
        char merged = colors[0] ^ colors[1] ^ colors[2];
        assert(merged == 'A' || merged == 'B');
        s.erase(left + 1, 2);
        if (merged == 'A')
          s[left] = 'B';
        else
          s[left] = 'A';
        colors[0] = s[left];
        right = left;
      }
    }

    --left;
  }
  if (last + 1 == (int)s.length()) {
    return false;
  }
  return solve(s);
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    int _;
    cin >> _;

    string s;
    cin >> s;
    // int na = 0;
    // const int n = s.length();
    // cout << "string len = " << n << endl;
    // for (char c : s)
    //   if (c == 'A')
    //     ++na;
    cout << "Case #" << i << ": ";
    if (solve(s))
      cout << 'Y';
    else
      cout << 'N';
    // cout << " number a, b = " << na << " " << n - na;
    cout << endl;
  }
  return 0;
}