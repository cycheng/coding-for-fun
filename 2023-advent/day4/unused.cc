/*
dp[i][j] = instances of card j at row = i.
dpsum[j] = all of copies (exclude original card) of card j at row = i + 1.

update order: i = start from last row, j = the card
if (win) dp[i][j] = 1 + dpsum[j]
else     dp[i][j] = 0

dpsum[j] += dp[i][j]

*/

// update order: i = start from last row, j = the card
//
// if (win) dp[i][j] = 1 + dpsum[j]
// else     dp[i][j] = 0

// dpsum[j] += dp[i][j]
// for (int i = curRow; i <= curRow + sum; ++i) {

for (uint32_t j = curRow + 1; j <= curRow + sum; ++j) {
  dp[i][j] = 1 + dpsum[j];
}
for (uint32_t j = curRow + 1; j <= curRow + sum; ++j)
  dpsum[j] += dp[i][j];

if (sum)
  dpsum[i] = 1;

cout << "----------------------\n";
cout << "(" << curRow << ")\n";
for (int v : dpsum)
  cout << v << " ";
cout << "\n\n";

for (auto &iter : dp) {
  for (int v : iter)
    cout << v << " ";
  cout << "\n";
}
cout << "\n";

//-------
//-------

//-------

for (uint32_t i = 1; i < numMatched; ++i) {
  uint32_t sum = 0;
  for (uint32_t j = 1; j <= i; ++j) {
    sum += dp[curRow + j][curRow + i];
  }
  dp[curRow][curRow + i] = sum;
}
