#include <iostream>
#include <vector>
#include <algorithm>

int solve(std::vector<std::vector<int>> &DP, std::vector<int> &coins, int i, int j, int m) {
  if (DP[i][j] != -1) {
    return DP[i][j];
  } else {
    if (j - i + 1 <= m) { 
      // if there are less than or equal to m coins remaining, we don't get a second turn
      DP[i][j] = std::max(coins[i], coins[j]); 
      return DP[i][j];
    } else {
      //either we take the left coin or we take the right coin.
      int right = -1;
      //interval is now [i,...,j-1] and we need to take (m-1) more coins out
      for (int l = 0; l < m; l++) { 
        if (right == -1) {
          right = coins[j] + solve(DP, coins, i+l, j-m+l, m);
        } else {
          right = std::min(right, coins[j] + solve(DP, coins, i+l, j-m+l, m));
        }
      }
      
      int left = -1;
      for (int l = 0; l < m; l++) {
        if (left == -1) {
          left = coins[i] + solve(DP, coins, i+m-l, j-l, m);
        } else {
          left = std::min(left, coins[i] + solve(DP, coins, i+m-l, j-l, m));
        }
      }
      
      DP[i][j] = std::max(left, right);
      return DP[i][j];
    }
  }
}

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  std::vector<int> coins(n, -1);
  
  for (int i = 0; i < n; i++) {
    int x; std::cin >> x;
    coins[i] = x;
  }
  
  //DP[i][j] stores money won by first player starting on array [i...j] regardless of what others do
  std::vector<std::vector<int>> DP(n, std::vector<int>(n, -1));
  
  // there are m players, and you are indexed k, so k players have gone before you.
  // so the interval should be shorter by k.
  // [0][n-1-k], [1][n-1-(k-1)]...[k][n-1] 
  
  for (int i = 0; i <= k; i++) {
    solve(DP, coins, 0 + i, n - 1 - k + i, m);
  }

  int ans = DP[k][n-1];
  for (int i = 0; i <= k; i++) {
    ans = std::min(ans, DP[0 + i][n - 1 - k + i]);
  }
  std::cout << ans << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}