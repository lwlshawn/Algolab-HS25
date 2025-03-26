#include <iostream>
#include <vector>
#include <algorithm>

void solve() {
  int n, m; std::cin >> n >> m;
  int r, b; std::cin >> r >> b;
  std::vector<std::vector<int>> adj_list(n, std::vector<int>());

  for (int i = 0; i < m; i++) {
    // i dont know why they did this. Vertices are 1-labelled.
    int u, v; std::cin >> u >> v; //edge from u up to v, u < v.
    adj_list[u-1].push_back(v-1);
  }
  
  // DP[pos][0] indicates # of moves to win starting "first"
  // DP[pos][1] indicates # of moves to win starting "second"
  std::vector<std::vector<int>> DP(n, std::vector<int>(2, -1));
  
  //base cases: DP[n-1][0/1] = 0;
  DP[n-1][0] = 0; DP[n-1][1] = 0;
  
  for (int pos = n-2; pos >= 0; pos--) { //for each position
    for (int adj : adj_list[pos]) { //look at all neighbours above us
      if (DP[pos][0] == -1) DP[pos][0] = 1 + DP[adj][1]; //if uninitialised
      else DP[pos][0] = std::min(DP[pos][0], 1 + DP[adj][1]);
      
      if (DP[pos][1] == -1) DP[pos][1] = 1 + DP[adj][0];
      else DP[pos][1] = std::max(DP[pos][1], 1 + DP[adj][0]);
    }
  }
  
  // std::cerr << "printing DP[pos][0]" << std::endl;
  // for (int i = 0; i < n; i++) {
  //   std::cerr << DP[i][0] << ' ';
  // }
  // std::cerr << std::endl;
  
  // std::cerr << "printing DP[pos][1]" << std::endl;
  // for (int i = 0; i < n; i++) {
  //   std::cerr << DP[i][1] << ' ';
  // }
  // std::cerr << std::endl;
  
  int sherlock = DP[r-1][0];
  int moriarty = DP[b-1][0];
  
  if (sherlock < moriarty) {
    std::cout << 0 << std::endl;
  } else if (moriarty < sherlock) {
    std::cout << 1 << std::endl;
  } else {
    if (sherlock % 2 == 0) {
      std::cout << 1 << std::endl;
    } else {
      std::cout << 0 << std::endl;
    }
  }
  
}

int main() {
  std::ios_base::sync_with_stdio(false);

  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}