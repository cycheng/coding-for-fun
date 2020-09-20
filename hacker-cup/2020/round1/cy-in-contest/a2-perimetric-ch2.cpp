#include <bits/stdc++.h>
using namespace std;

struct ReportExecTime {
  std::chrono::high_resolution_clock::time_point start;
  const char *prefix;
  ReportExecTime(const char *prefix = nullptr) : prefix(prefix) {
    start = std::chrono::high_resolution_clock::now();
  }
  ~ReportExecTime() {
    std::chrono::duration<double, std::milli> elapsed =
        std::chrono::high_resolution_clock::now() - start;
    std::cout << prefix << "Exe time = " << elapsed.count() << " msec."
              << std::endl;
  }
};

int solve() {
  constexpr int mod = 1e9 + 7;
  int n, k;
  cin >> n >> k;
  vector<int> left;
  left.reserve(n);
  for (int i = 0; i < k; ++i) {
    int l;
    cin >> l;
    left.emplace_back(l);
  }
  int al, bl, cl, dl;
  cin >> al >> bl >> cl >> dl;
  {
    // ReportExecTime t("get full l data: ");
    for (int i = k; i < n; ++i) {
      // Li = (al * Li-2 + bl * Li-1 + cl) % dl + 1
      uint64_t li = (uint64_t)al * (uint64_t)left[i - 2] +
                    (uint64_t)bl * (uint64_t)left[i - 1] + (uint64_t)cl;
      left.emplace_back((int)(li % dl) + 1);
    }
  }

  vector<int> width;
  width.reserve(n);
  for (int i = 0; i < k; ++i) {
    int w;
    cin >> w;
    width.emplace_back(w);
  }
  int aw, bw, cw, dw;
  cin >> aw >> bw >> cw >> dw;
  {
    // ReportExecTime t("get full width data: ");
    for (int i = k; i < n; ++i) {
      // wi = (aw * wi-2 + bw * wi-1 + cw) % dw + 1
      uint64_t wi = (uint64_t)aw * (uint64_t)width[i - 2] +
                    (uint64_t)bw * (uint64_t)width[i - 1] + (uint64_t)cw;
      width.emplace_back((int)(wi % dw) + 1);
    }
  }
  // cout << endl;
  // for (int l : left)
  //   cout << l << " ";
  // cout << endl;
  vector<int64_t> height;
  height.reserve(n);
  for (int i = 0; i < k; ++i) {
    int h;
    cin >> h;
    height.emplace_back(h);
  }
  int ah, bh, ch, dh;
  cin >> ah >> bh >> ch >> dh;
  {
    // ReportExecTime t("get full height data: ");
    for (int i = k; i < n; ++i) {
      // hi = (ah * hi-2 + bh * hi-1 + ch) % dh + 1
      uint64_t hi = (uint64_t)ah * (uint64_t)height[i - 2] +
                    (uint64_t)bh * (uint64_t)height[i - 1] + (uint64_t)ch;
      height.emplace_back((int)(hi % dh) + 1);
    }
  } // for (int h : height)
  //   cout << h << " ";
  // cout << endl;
  // calculate perimeter for each room, and culculate answer.
  // Pi = perimeter of the union of rooms 0..i

  // start, length.
  map<int64_t, int64_t> seg;
  seg[left[0]] = width[0];

  int64_t prevpi = width[0] * 2 + height[0] * 2;
  // cout << prevpi << " ";
  int64_t ptotal = prevpi;
  {

    // ReportExecTime t("compute time: ");
    for (int i = 1; i < n; ++i) {
      // auto front_it = seg.lower_bound(left[i]);
      auto front_it = seg.upper_bound(left[i]);
      auto end_it = seg.upper_bound(left[i] + width[i]);
      // if (front_it != seg.end() && front_it->first > left[i])
      //   --front_it;
      if (front_it != seg.end() && front_it != seg.begin())
        --front_it;

      // if (front_it == seg.end())
      //   front_it = seg.begin();

      if (front_it == seg.end())
        if (seg.size())
          front_it = --seg.end();

      bool is_new_seg = true;
      int64_t removed_width = 0;
      int64_t removed_height = 0;
      int64_t new_pos = left[i], new_len = width[i];
      vector<int64_t> remove_list;
      int64_t inner_loop_cnt = 0;
      for (; front_it != end_it; ++front_it) {
        ++inner_loop_cnt;
        // check if two segments overlap
        if (front_it->first + front_it->second < left[i] ||
            left[i] + width[i] < front_it->first) {
          continue;
        }

        // fully covered by current segment.
        //
        // problem statement: the union might not form a single connected
        // polygon
        if (front_it->first <= left[i] &&
            left[i] + width[i] <= front_it->first + front_it->second) {
          is_new_seg = false;
          new_len = 0;
          break;
        }
        removed_height %= mod;
        removed_width %= mod;

        // new segment fully covered current segment.
        // remove current segment.
        //
        // problem statement: the union might not form a single connected
        // polygon
        if (front_it->first + front_it->second <= left[i] + width[i] &&
            left[i] <= front_it->first) {
          removed_width += front_it->second;
          removed_height += height[i];
          remove_list.push_back(front_it->first);
          continue;
        }

        // merge overlapped segments.
        if (front_it->first < left[i]) {
          removed_width += front_it->second;
          removed_height += height[i];
          remove_list.push_back(front_it->first);
          new_pos = front_it->first;
          new_len += left[i] - front_it->first;
          // cout << "\noverlap remvoe 1\n";
        } else {
          assert(left[i] + width[i] < front_it->first + front_it->second);
          removed_width += front_it->second;
          removed_height += height[i];
          remove_list.push_back(front_it->first);
          new_len += front_it->first + front_it->second - left[i] - width[i];
          // cout << "\noverlap remvoe 2\n";
        }
      }
      // cout << "inner loop = " << inner_loop_cnt << endl;

      // if (remove_list.size())
      // cout << " erase size = " << remove_list.size();
      for (int rid : remove_list)
        seg.erase(rid);

      int64_t pi = 0;
      if (is_new_seg) {
        seg[new_pos] = new_len;
        pi = new_len * 2 + height[i] * 2;
      }

      removed_height %= mod;
      removed_width %= mod;
      pi %= mod;
      prevpi %= mod;

      // cout << "seg:\n";
      // for (auto it : seg)
      //   cout << "  " << it.first << ", " << it.first + it.second << endl;

      // assert(pi >= 0);
      pi = (pi + prevpi - 2 * removed_width - 2 * removed_height) % mod;
      if (pi < 0)
        pi += mod;
      // assert(pi >= 0);
      ptotal = (ptotal * pi) % mod;
      prevpi = pi;

      // cout << prevpi << " ";
    }
  }
  // cout << endl;

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