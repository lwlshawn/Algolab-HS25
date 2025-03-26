#include <iostream>
#include <vector>
#include <deque>
#include <set>
#include <algorithm>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;
typedef std::multiset<int> MSET;
typedef std::deque<int> DQ;
typedef std::set<int> SET;

void solve(VVI& adj_list, VI& brightness, MSET& values, DQ& old, DQ& window, SET& solutions, int root, int m, int k) {
  //add self to window
  window.push_back(root);
  values.insert(brightness[root]);
  
  //if window has size (m + 1)
  if (window.size() > m) {
    old.push_back(window.front());
    auto it = values.find(brightness[window.front()]);
    values.erase(it);
    window.pop_front();
  }
  
  //if window size is m, check if rope exists
  if (window.size() == m) {
    int smallest = *values.begin();
    int largest = *std::prev(values.end());
    if (largest - smallest <= k) {
      solutions.insert(window.front());
    }
  }
  
  for (int child : adj_list[root]) {
    solve(adj_list, brightness, values, old, window, solutions, child, m, k);
  }
  
  //now undo insertion of self into the data structures
  auto it = values.find(brightness[root]);
  values.erase(it);
  window.pop_back();
  
  if (old.size() > 0) {
    window.push_front(old.back());
    values.insert(brightness[old.back()]);
    old.pop_back();
  }
}

void testcase() {
  int n, m, k; std::cin >> n >> m >> k;
  std::vector<int> brightness;
  for (int i = 0; i < n; i++) {
    int h_i; std::cin >> h_i;
    brightness.push_back(h_i);
  }
  
  std::vector<std::vector<int>> adj_list(n);
  for (int i = 0; i < n-1; i++) {
    int u, v; std::cin >> u >> v;
    adj_list[u].push_back(v);
  }
  
  SET solutions;
  DQ old, window;
  MSET values;
  solve(adj_list, brightness, values, old, window, solutions, 0, m, k);

  if (solutions.size() == 0) {
    std::cout << "Abort mission" << std::endl;
  } else {
    for (int h : solutions) {
      std::cout << h << ' ';
    }
    std::cout << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}