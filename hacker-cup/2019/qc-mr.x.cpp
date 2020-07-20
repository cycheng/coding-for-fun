// https://www.facebook.com/codingcompetitions/hacker-cup/2019/qualification-round/problems/C
//
// #like, #25, #stack
//
// A single term, which is one of the following four characters:
//    'x'
//    'X' = -x
//    '0'
//    '1'
//
// A binary operator, which is one of the following three characters:
//    '|'
//    '&'
//    '^'
//
// Valid expression =
//      a single term, or
//      ([expression][operator][expression])
//
// Changing the expression so as to make x irrelevant!
// Find minimum number of characters which must be modified
//
// Constrains:
// You may not insert or delete characters. You may only change characters
//    in-place.
// 1 ≤ T ≤ 500
// 1 ≤ |E| ≤ 300
#include <bits/stdc++.h>
using namespace std;

char neg(char x) {
  if (x == 'x')
    return 'X';
  return 'x';
}

char eval(char a, char op, char b) {
  // ascii '0' = 0011 0000
  //       '1' = 0011 0001
  if (isdigit(a) && isdigit(b)) {
    switch (op) {
    case '&': return a & b;
    case '|': return a | b;
    case '^': return (a ^ b) + '0';
    default: cout << "impossible!" << endl;
    }
  }

  // one of the operand is 'x'
  if (isdigit(a) || isdigit(b)) {
    if (isdigit(a))
      swap(a, b);
    switch (op) {
    case '&':
      if (b == '1')
        return a;
      return '0';
    case '|':
      if (b == '1')
        return '1';
      return a;
    case '^':
      if (b == '1')
        return neg(a);
      return a;
    default: cout << "impossible!" << endl;
    }
  }

  // two operands are either 'x' or 'X'
  switch (op) {
  case '&':
    if (b == a)
      return a;
    return '0';
  case '|':
    if (b == a)
      return a;
    return '1';
  case '^':
    if (b == a)
      return '0';
    return '1';
  default: cout << "impossible!" << endl;
  }
  return -1;
}

int solve(string &s) {
  vector<char> op_stack;
  for (char c : s) {
    if (c == '(')
      ; // results.emplace_back('?');
    else if (c == ')') {
      char b = op_stack.back();
      op_stack.pop_back();

      char op = op_stack.back();
      op_stack.pop_back();

      char a = op_stack.back();
      op_stack.pop_back();
      op_stack.emplace_back(eval(a, op, b));
    } else {
      op_stack.emplace_back(c);
    }
  }
  if (op_stack.back() == '0' || op_stack.back() == '1')
    return 0;
  return 1;
}

int main(int argc, char *argv[]) {
  int n;
  string s;
  cin >> n;
  for (int i = 1; i <= n; ++i) {
    cin >> s;
    cout << "Case #" << i << ": " << solve(s) << endl;
  }
  return 0;
}