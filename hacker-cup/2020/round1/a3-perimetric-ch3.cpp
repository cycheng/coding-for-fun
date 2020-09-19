#include <bits/stdc++.h>
using namespace std;

constexpr int mod = 1e9 + 7;

vector<int> read(int n, int k) {
  vector<int> data(n);
  int i = 0;
  for (; i < k; ++i)
    cin >> data[i];
  int64_t a, b, c, d;
  cin >> a >> b >> c >> d;
  for (; i < n; ++i)
    data[i] = (int)((a * data[i - 2] + b * data[i - 1] + c) % d + 1);
  return data;
}

#ifdef DUMP
template <class T> void dumpPeri(const vector<T> &peri) {
  for (auto p : peri)
    cout << p << " ";
  cout << endl;
}
#else
template <class T> void dumpPeri(const vector<T> &peri) {}
#endif

vector<int64_t> horizontal(const vector<int> &left, const vector<int> &width) {
  const int n = left.size();
  vector<int64_t> perimetrics(n);

  int64_t periSum = 0;

  // <left, right>
  set<pair<int, int>> activeWidth;

  for (int i = 0; i < n; ++i) {
    int x1 = left[i], x2 = left[i] + width[i];
    while (true) {
      auto afterIt = activeWidth.lower_bound({x1, 0});
      if (afterIt == activeWidth.end() || afterIt->first > x2)
        break;
      periSum -= afterIt->second - afterIt->first;
      x2 = max(x2, afterIt->second);
      activeWidth.erase(afterIt);
    }

    auto afterIt = activeWidth.lower_bound({x1, 0});
    if (afterIt != activeWidth.begin()) {
      auto beforeIt = prev(afterIt);
      if (beforeIt->second >= x1) {
        periSum -= beforeIt->second - beforeIt->first;
        x1 = beforeIt->first;
        x2 = max(x2, beforeIt->second);
        activeWidth.erase(beforeIt);
      }
    }
    activeWidth.emplace(x1, x2);
    periSum += x2 - x1;
    perimetrics[i] = periSum * 2;
  }
  dumpPeri(perimetrics);
  return perimetrics;
}

namespace non_decreasing {

struct segment_tree {
  vector<int64_t> array;
  size_t n;

  segment_tree(size_t size) {
    n = size;
    array.resize(n * 2);
  }

  int64_t query(int pos) {
    assert((size_t)pos < n && "out of bound query.");
    int64_t ans = 0;
    for (pos += n; pos; pos >>= 1)
      ans = max(ans, array[pos]);
    return ans;
  }

  // update [left, right) with value.
  void update(int left, int right, int64_t value) {
    for (left += n, right += n; left < right; left >>= 1, right >>= 1) {
      if (left & 1) {
        array[left] = max(array[left], value);
        ++left;
      }
      if (right & 1) {
        --right;
        array[right] = max(array[right], value);
      }
    }
  }
};

struct discrete {
  // <original horizontal position, mapped id>
  unordered_map<int, int> hpos2id;

  discrete(const vector<int> &pos) {
    // position set
    set<int> pset(pos.begin(), pos.end());
    // unique postions
    vector<int> upos(pset.begin(), pset.end());
    for (uint32_t i = 0; i < upos.size(); ++i)
      hpos2id[upos[i]] = i;
  }

  int getid(int pos) const {
    auto iter = hpos2id.find(pos);
    assert(iter != hpos2id.end());
    return iter->second;
  }

  size_t size() const { return hpos2id.size(); }
};

struct point {
  int id;
  int64_t height;
  point() = default;
  point(int id, int64_t h) : id(id), height(h) {}
  bool operator<(const point &a) const { return this->id < a.id; }
};

// return vertical perimetric.
vector<int64_t> vertical(const vector<int> &left, const vector<int> &width,
                         const vector<int> &height) {
  const int n = left.size();
  vector<int64_t> perimetrics(n);

  vector<int> all_xpos;
  all_xpos.reserve(n * 2);
  all_xpos = left;
  for (int i = 0; i < n; ++i)
    all_xpos.push_back(left[i] + width[i]);

  discrete id(all_xpos);

  multiset<point> activeHeights;
  segment_tree sgt(id.size());
  int64_t periSum = 0;
  for (int i = 0; i < n; ++i) {
    int x1 = id.getid(left[i]), x2 = id.getid(left[i] + width[i]);
    int64_t x1h = height[i] - sgt.query(x1);
    int64_t x2h = height[i] - sgt.query(x2);

    while (activeHeights.size()) {
      auto it = activeHeights.lower_bound(point(x1, 0 /* don't care */));
      if (it == activeHeights.end() || it->id > x2)
        break;

      // merge the height.
      point mergePt = *it;
      activeHeights.erase(it);

      // Do not remove it->height from periSum:
      // case 1:          case 2:
      //   x1                   x2
      //   |                    |
      //   a1--a2           a1--a2
      //   |   |            |   |
      //   periSum += x1h   periSum += x2h
      // if ((mergePt.id == x1 && mergePt.height > 0) ||
      //     (mergePt.id == x2 && mergePt.height < 0))
      //   continue;

      // Need to remove it->height from periSum:
      // case 1:
      //   x1-------c--x2
      //   |  a  b  |  |
      //   |  |  |  |  |
      //   periSum = periSum - a - b - c
      //
      // case 2:          case 3:
      //       x1-------------x2
      //       |              |
      //   a1--a2             a1--a2
      //   |   |              |   |
      //   periSum -= a2+x1h  periSum -= a1+x2h
      periSum -= abs(mergePt.height);
      if (mergePt.id == x1 && mergePt.height > 0)
        x1h += mergePt.height;
      if (mergePt.id == x2 && mergePt.height < 0)
        x2h -= mergePt.height;
    }
    periSum += x1h;
    periSum += x2h;
    activeHeights.emplace(x1, x1h);  // height[i]);
    activeHeights.emplace(x2, -x2h); //-height[i]);

    sgt.update(x1, x2 + 1, height[i]);
    perimetrics[i] = periSum;
  }
  dumpPeri(perimetrics);
  return perimetrics;
}

} // namespace non_decreasing
int solve() {
  int n, k;
  cin >> n >> k;
  vector<int> left = read(n, k);
  vector<int> width = read(n, k);
  vector<int> height = read(n, k);

  vector<int64_t> vperi = non_decreasing::vertical(left, width, height);
  vector<int64_t> hperi = horizontal(left, width);

  int64_t ans = 1;
  for (int i = 0; i < n; ++i) {
    int64_t p = (vperi[i] + hperi[i]) % mod;
    ans = (ans * p) % mod;
  }
  return ans;
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