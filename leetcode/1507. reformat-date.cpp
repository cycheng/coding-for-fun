// https://leetcode.com/contest/biweekly-contest-30/problems/reformat-date/
//
// #biweekly 30
// #string, #3

// Given date string: Day Month Year
// Day: {"1st", "2nd", "3rd", "4th", ..., "30th", "31st"}
// Month: {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct",
//         "Nov", "Dec"}
// Year: [1900, 2100]
// Convert to YYYY-MM-DD
#include "std.h"

string reformatDate(string date) {
  // see:
  // https://leetcode.com/problems/reformat-date/discuss/730625/C%2B%2B-(Less-Typing-More-Fun!)
  stringstream ss(date);
  string d, m, y;
  ss >> d >> m >> y;
  string ans = y + "-";

  const char *mstr[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
  for (int i = 0; i < 12; ++i)
    if (m.compare(mstr[i]) == 0) {
      if (i < 9)
        ans += "0";
      ans += to_string(i + 1) + "-";
    }
  while (!isdigit(d.back()))
    d.pop_back();
  if (d.size() == 1)
    ans += "0";
  ans += d;
  return ans;
}
