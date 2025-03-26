#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <cassert>

int dfs(int v, int t, int n, std::vector<int> &dt, std::vector<int> &min_dt,
std::vector<std::vector<int>> &adj_list, std::vector<std::pair<int, int>> &critical,
std::vector<int> &pred) {
  int curr_t = t;
  dt[v] = curr_t;
  min_dt[v] = dt[v];
  
  for (int u : adj_list[v]) { // for each neighbour u of v
    if (dt[u] == -1) { // if neighbour is not discovered
      pred[u] = v;
      curr_t = dfs(u, curr_t + 1, n, dt, min_dt, adj_list, critical, pred);
      min_dt[v] = std::min(min_dt[v], min_dt[u]);
      
      //mark edge critical if correct to do so.
      if (min_dt[u] > dt[v]) {
        // if dfs from u does not see smaller discovery time then dt v, edge u, v critical
        if (u <= v) {
          critical.push_back(std::pair<int, int>(u, v));
        } else {
          critical.push_back(std::pair<int, int>(v, u));
        }
      }
    } else {
      //neighbour discovered before, mark discovery time if NOT pred
      if (u != pred[v]) min_dt[v] = std::min(min_dt[v], dt[u]);
    }
    
  }
  return curr_t;
}

void solve() {
  int n, m; std::cin >> n >> m;
  
  if (n == 0) { //why?
    std::cout << 0 << std::endl;
    return;
  }
  
  std::vector<std::vector<int>> adj_list(n, std::vector<int>());
  std::vector<int> pred(n, -1);
  std::vector<int> dt(n, -1);
  std::vector<int> min_dt(n, n);
  std::vector<std::pair<int, int>> critical;
  
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    adj_list[u].push_back(v);
    adj_list[v].push_back(u);
  }
  
  dfs(0, 0, n, dt, min_dt, adj_list, critical, pred);
  std::cout << critical.size() << std::endl;
  
  std::sort(critical.begin(), critical.end());
  for (std::pair<int, int> e : critical) {
    std::cout << e.first << ' ' << e.second << std::endl;
  }
  
  // std::cerr << "discovery times " << std::endl;
  // for (int t : dt) {
  //   std::cerr << t << ' ';
  // }
  // std::cerr << std::endl;
  
  // std::cerr << "min discovery times " << std::endl;
  // for (int t : min_dt) {
  //   std::cerr << t << ' ';
  // }
  // std::cerr << std::endl;

}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}