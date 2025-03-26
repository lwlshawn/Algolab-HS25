#include <iostream>
#include <vector>
#include <cmath>


void testcase() {
  int n, k; std::cin >> n >> k;
  std::vector<int> cards;
  for (int i = 0; i < n; i++) {
    int v_i; std::cin >> v_i;
    cards.push_back(v_i);
  }
  
  int l = 0; 
  int r = 0;
  int sum = cards[0];
  
  int best_l = 0; 
  int best_r = 0;
  int best_diff = std::abs(k - sum);

  if (sum == k) {
    std::cout << 0 << ' ' << 0 << std::endl;
    return;
  }
  
  while (r < n) {
    if (sum < k) {
      r++;
      if (r == n) break;
      sum += cards[r];
    } else if (sum >= k) {
      sum -= cards[l];
      l++;
    }
    
    if (std::abs(k - sum) < best_diff) {
      best_l = l;
      best_r = r;
      best_diff = std::abs(k - sum);
    }
  }
  std::cout << best_l << ' ' << best_r << std::endl;
  
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}