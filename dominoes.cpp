#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n; std::cin >> n;
  std::vector<int> dominoes;
  
  for (int i = 0; i < n; i++) {
    int h; std::cin >> h;
    dominoes.push_back(h);
  }
  
  // when a domino with height h_i at position i falls,
  // it causes all dominoes at positions j, such that
  // j < h_i + i will fall.
  
  long count = dominoes[0]; //stores h_i + i
  for (int j = 1; j < n; j++) {
    if (j >= count) break;
    
    // else, j < h_i + i. So current domino falls.
    if (count < dominoes[j] + j) {
      count = std::min(dominoes[j] + j, n);
    }
  }
  std::cout << count << std::endl;
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}