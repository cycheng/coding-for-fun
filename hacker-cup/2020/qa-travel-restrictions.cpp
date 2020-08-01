#include <bits/stdc++.h>
using namespace std;

void solve() {
  int n;
  cin >> n;
  string in, out;
  cin >> in >> out;
  bool ans[n][n];
  memset(ans, 0, sizeof(ans));
  for (int i = 0; i < n; ++i)
    ans[i][i] = true;

  // update upper right triangle.
  for (int i = n - 2; i >= 0; --i) {
    if (out[i] == 'N' || in[i + 1] == 'N')
      continue;

    ans[i][i + 1] = true;
    for (int j = i + 2; j < n; ++j)
      if (ans[i][j - 1] && ans[i + 1][j])
        ans[i][j] = true;
  }

  // update lower left triangle.
  for (int i = 1; i < n; ++i) {
    if (out[i] == 'N' || in[i - 1] == 'N')
      continue;

    ans[i][i - 1] = true;
    for (int j = i - 2; j >= 0; --j)
      if (ans[i][j + 1] && ans[i - 1][j])
        ans[i][j] = true;
  }

  // print result
  for (int i = 0; i < n; ++i) {
    cout << endl;
    for (int j = 0; j < n; ++j)
      if (ans[i][j])
        cout << 'Y';
      else
        cout << 'N';
  }
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cout << "Case #" << i << ": ";
    solve();
    cout << endl;
  }
  return 0;
}