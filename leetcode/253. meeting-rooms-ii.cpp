// https://leetcode.com/problems/meeting-rooms-ii/
//
// g++ -Wall -std=c++14 -O3 -g 253.\ meeting-rooms-ii.cpp
#include "std.h"

bool hasOverlap(const vector<int> &t0, const vector<int> &t1) {
  return !(t0[1] <= t1[0] || t1[1] <= t0[0]);
}

// cy second attempt, but it is still O(n^2)
int minMeetingRooms_nSquare(vector<vector<int>> &intervals) {
  sort(intervals.begin(), intervals.end(),
       [](const vector<int> &a, const vector<int> &b) { return a[0] < b[0]; });

  if (intervals.empty())
    return 0;

  vector<int> curCollisionId;
  int minRooms = 1;

  curCollisionId.push_back(0);
  // worst case is still O(n^2)
  for (int i = 1; i < intervals.size(); ++i) {
    const auto time = intervals[i];
    for (int j = curCollisionId.size() - 1; j >= 0; --j) {
      int id = curCollisionId[j];
      if (!hasOverlap(intervals[id], time))
        curCollisionId.erase(curCollisionId.begin() + j);
    }
    curCollisionId.push_back(i);
    minRooms = max(minRooms, (int)curCollisionId.size());
  }
  return minRooms;
}

// better NlogN solutions
int minMeetingRooms_nlogn(vector<vector<int>> &intervals) {
  map<int, int> timeToRooms;
  for (const auto t : intervals) {
    timeToRooms[t[0]]++;
    timeToRooms[t[1]]--;
  }
  int minRooms = 0, curRooms = 0;
  for (const auto r : timeToRooms) {
    curRooms += r.second;
    minRooms = max(minRooms, curRooms);
  }
  return minRooms;
}

int minMeetingRooms(vector<vector<int>> &&intervals) {
  // return minMeetingRooms_nSquare(intervals);
  return minMeetingRooms_nlogn(intervals);
}

void expect(int ref, int result) {
  if (ref != result)
    cout << "Failure" << endl;
  else
    cout << "Success" << endl;
}

int main(int argc, char *argv[]) {
  expect(2, minMeetingRooms({{0, 30}, {5, 10}, {15, 20}}));
  expect(1, minMeetingRooms({{7, 10}, {1, 3}, {5, 6}}));
  expect(1, minMeetingRooms({{5, 7}, {1, 3}, {3, 5}}));
  expect(4, minMeetingRooms({{0, 30}, {0, 30}, {0, 30}, {0, 30}}));
  return 0;
}
