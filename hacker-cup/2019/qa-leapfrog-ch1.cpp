// https://www.facebook.com/codingcompetitions/hacker-cup/2019/qualification-round/problems/A
// Constraints
// 1 ≤ T ≤ 500
// 2 ≤ N ≤ 5,000
#include <bits/stdc++.h>
using namespace std;

char solve(string &s) {
  int numb = 0;
  for (char c : s)
    if (c == 'B')
      ++numb;
  if (numb < s.length() - numb - 1 || numb == s.length() - 1)
    return 'N';
  return 'Y';
}

int main(int argc, char *argv[]) {
  int n;
  vector<string> s;
  cin >> n;
  for (int i = 0; i < n; ++i) {
    string t;
    cin >> t;
    s.emplace_back(t);
  }
  for (int i = 0; i < n; ++i)
    cout << "Case #" << i + 1 << ": " << solve(s[i]) << endl;

  return 0;
}