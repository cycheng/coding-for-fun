#include <bits/stdc++.h>
using namespace std;

int solve() {
  constexpr int mod = 1e9 + 7;
  int n, k, w;
  cin >> n >> k >> w;
  vector<int> left;
  left.reserve(n);
  for (int i = 0; i < k; ++i) {
    int l;
    cin >> l;
    left.emplace_back(l);
  }
  int al, bl, cl, dl;
  cin >> al >> bl >> cl >> dl;
  for (int i = k; i < n; ++i) {
    // Li = (al * Li-2 + bl * Li-1 + cl) % dl + 1
    uint64_t li = (uint64_t)al * (uint64_t)left[i - 2] +
                  (uint64_t)bl * (uint64_t)left[i - 1] + (uint64_t)cl;
    left.emplace_back((int)(li % dl) + 1);
  }
  vector<int> height;
  height.reserve(n);
  for (int i = 0; i < k; ++i) {
    int h;
    cin >> h;
    height.emplace_back(h);
  }
  int ah, bh, ch, dh;
  cin >> ah >> bh >> ch >> dh;
  for (int i = k; i < n; ++i) {
    // hi = (ah * hi-2 + bh * hi-1 + ch) % dh + 1
    uint64_t hi = (uint64_t)ah * (uint64_t)height[i - 2] +
                  (uint64_t)bh * (uint64_t)height[i - 1] + (uint64_t)ch;
    height.emplace_back((int)(hi % dh) + 1);
  }

  // calculate perimeter for each room, and culculate answer.
  // Pi = perimeter of the union of rooms 0..i
  int64_t prevpi = 0;
  int64_t ptotal = 1;
  for (int i = 0; i < n; ++i) {
    int64_t pi = 0;

    // non-overlap case:
    if (i == 0 || left[i] > left[i - 1] + w) {
      pi = (2 * height[i] + 2 * w); // % mod;
    } else {
      // w = [1, 20]
      pi = 0;
      pi += (left[i] - left[i - 1]) * 2;
      // search at most w steps to find additonal height we need to add
      int additionalH = numeric_limits<int>::max();
      for (int j = i - 1; j >= 0; --j) {
        if (left[i] > left[j] + w)
          break;
        if (height[i] < height[j]) {
          additionalH = 0;
          break;
        }
        additionalH = min(additionalH, height[i] - height[j]);
      }
      pi += additionalH * 2;
    }
    pi = (pi + prevpi) % mod;
    ptotal = (ptotal * pi) % mod;
    prevpi = pi;
  }
  cout << endl;
  return ptotal;
}

int main(int argc, char *argv[]) {
  int t;
  cin >> t;
  for (int i = 1; i <= t; ++i) {
    cout << "Case #" << i << ": " << solve();
    cout << endl;
  }
  return 0;
}