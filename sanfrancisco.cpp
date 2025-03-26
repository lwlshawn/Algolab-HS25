///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <tuple>
#include <cassert>


void testcase() {
  long n, m, x, k; std::cin >> n >> m >> x >> k;
  
  //DP[i][k]: max score from pos i in k moves
  //base cases are DP[i][0] which are always 0, and DP[i][1] where you just take maximum available move.
  //DP[i][1] treated differently if i is a sink.
  std::vector<std::vector<long>> DP(n, std::vector<long>(k+1, 0)); 
  std::vector<std::vector<std::pair<long, long>>> nbrs(n);
  for (int i = 0; i < m; i++) {
    long u, v, p; std::cin >> u >> v >> p;
    nbrs[u].emplace_back(v, p); //nbrs[u] is a vector of pairs(nbr, points)
  }
  
  long pts;
  for (int i = 0; i < n; i++) {
    if (nbrs[i].empty()) {
      DP[i][1] = DP[0][1];
    } else {
      for (auto p : nbrs[i]) {
        pts = p.second;
        DP[i][1] = std::max(DP[i][1], pts);
        
        if (i==0 && DP[0][1] >= x) {
          std::cout << 1 << std::endl;
          return;
        }
      }      
    }
  }
  
  for (int j = 2; j <= k; j++) { //with j moves
    for (int i = 0; i < n; i++) { //at position i
      //if hole is a sink
      if (nbrs[i].empty()) {
        DP[i][j] = DP[0][j]; //move it back to the start position
      } else {
        for (auto p : nbrs[i]) { //for each neighbour
          long nbr = p.first;
          pts = p.second;
          
          if (i == 0 && pts + DP[nbr][j-1] >= x) {
            std::cout << j << std::endl;
            return;
          } //overflow segfault without early termination, even with long somehow
          
          DP[i][j] = std::max(DP[i][j], pts + DP[nbr][j-1]); 
        }        
      }
    }
  }

  std::cout << "Impossible" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}