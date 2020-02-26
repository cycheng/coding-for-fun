// https://leetcode.com/problems/meeting-rooms/
//
// g++ -Wall -std=c++14 -O3 -g 252.\ meeting-rooms.cpp
#include "std.h"

bool canAttendMeetings(vector<vector<int>> &intervals) {
  sort(intervals.begin(), intervals.end(),
       [](const vector<int> &a, const vector<int> &b) { return a[0] < b[0]; });
  for (uint32_t i = 1; i < intervals.size(); ++i)
    if (intervals[i - 1][1] > intervals[i][0])
      return false;
  return true;
}

bool canAttendMeetings(vector<vector<int>> &&intervals) {
  return canAttendMeetings(intervals);
}

void expect(bool direction, bool result) {
  if (direction != result)
    cout << "Failure" << endl;
  else
    cout << "Success" << endl;
}

int main(int argc, char *argv[]) {
  expect(false, canAttendMeetings({{0, 30}, {5, 10}, {15, 20}}));
  expect(true, canAttendMeetings({{7, 10}, {1, 3}, {5, 6}}));
  expect(true, canAttendMeetings({{5, 7}, {1, 3}, {3, 5}}));
  return 0;
}
