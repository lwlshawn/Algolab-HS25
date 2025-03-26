#include <iostream>
#include <vector>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

void testcase() {
  int n, m; std::cin >> n >> m;
  
  std::vector<std::vector<std::pair<int, long>>> outgoing_edges(n);
  std::vector<std::vector<std::pair<int, long>>> incoming_edges(n);
  for (int i = 0; i < m; i++) {
    int u, v; long f; std::cin >> u >> v >> f;
    outgoing_edges[u].push_back(std::pair<int, long>(v, f));
    incoming_edges[v].push_back(std::pair<int, long>(u, f)); 
  }
  
  //DP[i][j] stores max score of a path from 0 up to i, and j down to 0 with no re-use of vertices except
  //possiblly vertex n-1
  std::vector<std::vector<long>> DP(n, std::vector<long>(n, -1));
  for (auto p : outgoing_edges[0]) {
    int v = p.first; long f = p.second;
    DP[v][0] = f;
  }
  
  for (auto p : incoming_edges[0]) {
    int v = p.first; long f = p.second;
    DP[0][v] = f;
  }
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (DP[i][j] != -1) {
        for (auto p : outgoing_edges[i]) {
          int v = p.first; long f = p.second;
          if (v < j) continue;
          if (v == j && v != n -1) continue;
          DP[v][j] = std::max(DP[v][j], DP[i][j] + f);
        }
        
        for (auto p :incoming_edges[j]) {
          int v = p.first; long f = p.second;
          if (v < i) continue;
          if (v == i && v != n-1) continue;
          DP[i][v] = std::max(DP[i][v], DP[i][j] + f);
        }
      }
    }
  }
  
  std::cout << DP[n-1][n-1] << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}