///3
#include <iostream>
#include <vector>
#include <algorithm>

void testcase() {
  // std::cerr << "testcase begins" << std::endl;
  int n, m; std::cin >> n >> m;
  long a, b; std::cin >> a >> b;
  long P, H, W; std::cin >> P >> H >> W;
  
  std::vector<long> power;
  std::vector<long> happiness;
  for (int i = 0; i < n; i++) {
    long p_i, h_i; std::cin >> p_i >> h_i;
    power.push_back(p_i);
    happiness.push_back(h_i);
  }
  
  std::vector<long> wit;
  for (int i = 0; i < m; i++) {
    long w_j; std::cin >> w_j;
    wit.push_back(w_j);
  }
  
  //DP[h][i][j] denotes maximum power attainable, with happiness at least h, taking exactly j of the first i potions
  //entries are -1 for j > i
  std::vector<std::vector<std::vector<long>>> DP(H+1, std::vector<std::vector<long>>(n + 1, std::vector<long>(n + 1, -1)));
  for (int i = 0; i <= n; i++) {
    DP[0][i][0] = 0; //0 power achieved with 0 happiness taking 0 of the first i potions
  }
  
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= i; j++) {
      for (int h = 0; h <= H; h++) {
        int h_ind = std::max(0, (int)(h - happiness[i-1]));
        // std::cerr << i << ' ' << j << std::endl;
        // std::cerr << "h_ind: " << h_ind << std::endl;
        // std::cerr << DP[h_ind][i-1][j-1] << std::endl;
        if (DP[h_ind][i-1][j-1] == -1) {
          DP[h][i][j] = DP[h][i-1][j];
        } else {
          DP[h][i][j] = std::max(DP[h][i-1][j], DP[h_ind][i-1][j-1] + power[i-1]);
        }
        
        // std::cerr << "DP[h][i][j]: " << DP[h][i][j] << std::endl;
      }
    }
  }
  
  // for (long i = 0; i <= n; i++) {
  //   std::cerr << DP[H][n][i] << ' ';
  // }
  // std::cerr << std::endl;
  
  std::sort(wit.begin(), wit.end(), std::greater<long>());
  long curr_wit = 0;
  for (int i = 0; i < m; i++) { //taking i+1 potions of type B
    curr_wit += wit[i];
    if (curr_wit < W) continue;
    //we've taken i wit potions, losing us i * b amount of magical power
    for (long j = 1; j <= n; j++) {
      if (DP[H][n][j] == -1) continue;
      if ((DP[H][n][j] >= (P + (i+1) * b)) && curr_wit - (j * a) >= W) {
        // std::cerr << "j is: " << j  << std::endl;
        // std::cerr << "DP[H][j][j]: " << DP[H][j][j] << std::endl;
        // std::cerr << (P - (i+1) * b) << std::endl;
        // std::cerr << "curr_wit is: " << curr_wit << std::endl;
        std::cout << i + j + 1 << std::endl;
        return;
      }
    }
  }
  
  std::cout << -1 << std::endl;
}

/*
really tricky to implement with several pitfalls
- indexing of potions is a -1
- long! P can be 2^31, so P + (n*b) certainly need not fit into a long
- the DP is EXACTLY j of the first i potions, so that we can sum i + j to get num potions
*/



int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}