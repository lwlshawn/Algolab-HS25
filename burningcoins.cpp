#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n; std::cin >> n;
  std::vector<int> coins(n, -1);
  
  for (int i = 0; i < n; i++) {
    int v; std::cin >> v;
    coins[i] = v;
  }
  
  std::vector<int> prefixSum(n, -1);
  prefixSum[0] = coins[0];
  
  for (int i = 1; i < n; i++) {
    prefixSum[i] = coins[i] + prefixSum[i - 1];
  }
  
  std::vector<std::vector<int>> DP(n, std::vector<int>(n, -1));
  //base cases
  for (int i = 0; i < n; i++) {
    DP[i][i] = coins[i];
  }
  
  for (int i = n - 2; i >= 0; i--) {
    for (int j = i + 1; j < n; j++) {
      if (i != 0) {
        DP[i][j] = std::max(coins[i] + (prefixSum[j] - prefixSum[i]) - DP[i+1][j], 
                            coins[j] + (prefixSum[j - 1] - prefixSum[i - 1]) - DP[i][j-1]);        
      } else {
        DP[i][j] = std::max(coins[i] + (prefixSum[j] - prefixSum[i]) - DP[i+1][j], 
                            coins[j] + prefixSum[j - 1] - DP[i][j-1]);         
      }

    }
  }
  
  std::cout << DP[0][n-1] << std::endl;
  
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}