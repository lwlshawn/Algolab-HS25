#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n, m; long k; std::cin >> n >> m >> k;
  std::vector<int> defenders(n, -1);
  
  for (int i = 0; i < n; i++) {
    int v; std::cin >> v;
    defenders[i] = v;
  }
  
  //for each defender v_r, if there exists an index l <= r such that
  // v_l, v_{l+1},...,v_r sums to k, i want to store this index l, o/w -1.
  std::vector<int> left_end(n, -1);
  int l, r; long sum; l = r = sum = 0;
  
  while (r < n) {
    while(r < n && sum < k) {
      sum += defenders[r];
      r++;
    }
    
    if (sum == k) { //sum of defenders v_l up to v_{r-1} is k
      left_end[r-1] = l;
    }
    
    while (sum >= k) {
      sum -= defenders[l];
      l++;
      
      if (sum == k) {
        left_end[r-1] = l;
      }
    }
  }

  std::vector<std::vector<int>> DP(m + 1, std::vector<int>(n, -1));
  for (int j = 0; j < n; j++) {
    DP[0][j] = 0; //0 attackers is a valid strategy
  }
  
  for (int i = 1; i <= m; i++) { //for 1 or more attackers
    for (int j = 0; j < n; j++) { //for defenders v_0 up to v_j
      if (j == 0) { //this condition also needed to recurse for DP
        // here i'm assuming that i should fail the strategy if i have leftover attackers
        if (defenders[0] == k && i == 1) DP[1][0] = 1;
      } else {
        if (left_end[j] != -1) { //if i can use current defender as a right end
          if (left_end[j] == 0) { //if this requires me to use all defenders
            if (i == 1) { //only valid if i have exactly one attacker
              DP[i][j] = j - left_end[j] + 1;
            }
          } else { //left_end[j] != -1 && left_end[j] != 0
            if (DP[i-1][left_end[j] - 1] != -1) { //able to use defender as right end, and valid
              DP[i][j] = std::max(DP[i][j-1], DP[i-1][left_end[j] - 1] + (j - left_end[j] + 1));
            } else {
              DP[i][j] = DP[i][j-1];
            }
          }
          
        } else { //cant use current defender as right end anyway so recurse
          DP[i][j] = DP[i][j-1];
        }
      }
    }
  }
  
  if (DP[m][n-1] == -1) std::cout << "fail" << std::endl;
  else std::cout << DP[m][n-1] << std::endl;
  
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}