// https://adventofcode.com/2023/day/24

#include <bits/stdc++.h>
using namespace std;

enum { X = 0, Y, Z };

struct Stone {
  uint64_t pos[3] = {0, 0, 0};
  int vel[3] = {0, 0, 0};
  void print() const {
    cout << "pos " << pos[0] << " " << pos[1] << " " << pos[2] << ", vel "
         << vel[0] << " " << vel[1] << " " << vel[2] << "\n";
  }
};

constexpr double Epsilon = 1e-6;
constexpr double MinBound = 200000000000000;
constexpr double MaxBound = 400000000000000;

// ref:
// https://zhuanlan.zhihu.com/p/158533421
// https://en.wikipedia.org/wiki/Dot_product
// https://en.wikipedia.org/wiki/Sine_and_cosine
void solve(vector<Stone> &inputs) {
  int N = inputs.size();
  int ans = 0;
  for (int i = 0; i < N; ++i) {
    const Stone &si = inputs[i];
    double mi = (double)si.vel[Y] / si.vel[X];
    double ai = mi;
    double bi = ((double)si.pos[Y] - ai * si.pos[X]);

    // y = a0 x + b0
    // y = a1 x + b1
    for (int j = i + 1; j < N; ++j) {
      const Stone &sj = inputs[j];
      double mj = (double)sj.vel[Y] / sj.vel[X];
      double aj = mj;
      double bj = ((double)sj.pos[Y] - aj * sj.pos[X]);

      // two lines are parallel
      if (abs(mi - mj) <= Epsilon) {
        cout << "parallel:\n";
        cout << "  ";
        si.print();
        cout << "  ";
        sj.print();
        continue;
      }

      // 2d line function:
      // ax + by = e,
      // cx + dy = f,
      double a = -ai, b = 1.0, e = bi;
      double c = -aj, d = 1.0, f = bj;

      // find intersect point:
      //     | e b |      | a e |
      //     | f d |      | c f |
      // x = -------, y = -------
      //     | a b |      | a b |
      //     | c d |      | c d |
      double x = (e * d - b * f) / (a * d - b * c);
      double y = (a * f - e * c) / (a * d - b * c);

      // get vector for (x, y) from PosI and PosJ respectively.
      double veci[2] = {x - si.pos[X], y - si.pos[Y]};
      double vecj[2] = {x - sj.pos[X], y - sj.pos[Y]};

      // dot product of two vectors => cos theta < 0 => different direction.
      if (veci[X] * si.vel[X] + veci[Y] * si.vel[Y] < 0)
        continue;
      if (vecj[X] * sj.vel[X] + vecj[Y] * sj.vel[Y] < 0)
        continue;
      if (x < MinBound || MaxBound < x || y < MinBound || MaxBound < y)
        continue;

      cout << "intersection " << x << " " << y << "\n";
      ans += 1;
    }
  }
  cout << "ans = " << ans << "\n";
}

// Other good solutions:
// https://github.com/TSoli/advent-of-code/blob/main/2023/day24a/solution.cpp
// https://github.com/TSoli/advent-of-code/blob/main/2023/day24b/solution.cpp
// https://github.com/tmoux/AdventOfCode-23/tree/master/Day24

void parse(vector<Stone> &inputs, const string &line) {
  // 20, 19, 15 @  1, -5, -3
  stringstream sstrm(line);
  Stone p;
  char tmp;
  for (int i = 0; i < 3; ++i) {
    sstrm >> p.pos[i];
    sstrm >> tmp;
  }

  for (int i = 0; i < 3; ++i) {
    sstrm >> p.vel[i];
    sstrm >> tmp;
  }
  inputs.push_back(p);
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  vector<Stone> inputs;
  for (std::string line; getline(input, line);) {
    parse(inputs, line);
  }
  solve(inputs);
  return 0;
}