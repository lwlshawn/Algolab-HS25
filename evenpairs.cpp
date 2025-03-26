#include <iostream>
#include <vector>

// need to be able to solve this in linear time. some kind of DP probably.
// new idea: i just need to count number of even parities, and number
// of odd parities.
int nChooseTwo(int n) {
  return (n * (n - 1)) / 2;
}

void solve() {
  int n; std::cin >> n;
  int even_parities = 0;
  int odd_parities = 0;
  int current_parity = 0;
  
  for (int i = 0; i < n; i++) {
    int x; std::cin >> x;
    current_parity = (current_parity + x) % 2;
    
    if (current_parity == 0) {
      even_parities += 1;
    } else {
      odd_parities += 1;
    }
  }
  std::cout << even_parities + nChooseTwo(even_parities) + nChooseTwo(odd_parities) << std::endl;
}

int main() {
  int t; std::cin >> t;
  for(int i = 0; i < t; i++) {
    solve();
  }
}