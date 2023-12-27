// https://adventofcode.com/2023/day/18

#include <bits/stdc++.h>
using namespace std;

// https://www.geeksforgeeks.org/area-of-a-polygon-with-given-n-ordered-vertices/

using Vertices = vector<pair<int, int>>;
void buildVertices(int &x, int &y, const string &line, Vertices &vers,
                   int &lineLen) {
  char dir = line[0];
  int step = stoi(line.substr(1, line.find('(')));
  switch (dir) {
  case 'U': y -= step; break;
  case 'D': y += step; break;
  case 'L': x -= step; break;
  case 'R': x += step; break;
  default: assert(false);
  }
  lineLen += step;
  vers.emplace_back(y, x);
}

// (X[i], Y[i]) are coordinates of i'th point.
int polygonArea(const Vertices &vert) {
  // Initialize area
  int area = 0;

  // Calculate value of shoelace formula
  int j = vert.size() - 1;
  for (int i = 0; i < (int)vert.size(); i++) {
    int Xi = vert[i].second, Yi = vert[i].first;
    int Xj = vert[j].second, Yj = vert[j].first;

    area += (Xj + Xi) * (Yj - Yi);
    j = i; // j is previous vertex to i
  }

  // Return absolute value
  return abs(area / 2);
}

int main(int argc, char *argv[]) {
  std::ifstream input(argv[1]);
  Vertices vert;
  int perimeter = 0;
  int x = 0, y = 0;
  for (std::string line; getline(input, line);) {
    buildVertices(x, y, line, vert, perimeter);
  }

  cout << "ans = " << polygonArea(vert) + perimeter / 2 + 1 << "\n";

  return 0;
}
