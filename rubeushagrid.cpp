///4
#include <iostream>
#include <utility>
#include <vector>
#include <algorithm>
#include <tuple>

typedef std::pair<int, int> P;
typedef std::vector<P> VP;
typedef std::vector<VP> VVP;

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

typedef std::vector<long> VL;

void compute_stats(VVP& adj_list, VL& sum_tunnels, VL& sum_rooms, VI& galleons, int root) {
  // std::cerr << "compute_stats called on: " << root << std::endl;
  for (auto p : adj_list[root]) {
    int child = p.first;
    compute_stats(adj_list, sum_tunnels, sum_rooms, galleons, child);
  }
  
  long sum_tree_tunnels = 0, sum_tree_rooms = 1;
  for (auto p : adj_list[root]) {
    int child = p.first;
    long tun_child = p.second;
    
    sum_tree_tunnels += tun_child + sum_tunnels[child];
    sum_tree_rooms += sum_rooms[child];
  }
  
  sum_rooms[root] = sum_tree_rooms;
  sum_tunnels[root] = sum_tree_tunnels;
}

long solve(VVP& adj_list, VL& sum_tunnels, VL& sum_rooms, VI& galleons, int root, long& t) {
  //starting at time t, computes the amount of gold retrieved from the subtree rooted at root.
  std::vector<std::tuple<double, long, int>> averages;
  for (auto p : adj_list[root]) {
    int child = p.first;
    long tun_to_child = p.second;
    
    double sum_child_tunnels = sum_tunnels[child] + tun_to_child;
    double sum_child_rooms = sum_rooms[child];
    double child_average = sum_child_rooms / sum_child_tunnels;
    averages.emplace_back(child_average, tun_to_child, child);
  }
  
  std::sort(averages.begin(), averages.end(), std::greater<std::tuple<double, long, int>>());
  
  long gold_obtained = galleons[root] - t;
  for (auto tup : averages) {
    int child = std::get<2>(tup);
    long tun_to_child = std::get<1>(tup);
    
    t += tun_to_child;
    gold_obtained += solve(adj_list, sum_tunnels, sum_rooms, galleons, child, t);
    t += tun_to_child;
  }
  return gold_obtained;
}


void testcase() {
  //tunnels are labelled from 0 UP TO n
  // std::cerr << "testcase begins" << std::endl;
  int n; std::cin >> n;
  
  std::vector<int> galleons(n+1);
  galleons[0] = 0;
  for (int i = 0; i < n; i++) {
    int g_i; std::cin >> g_i;
    galleons[i+1] = g_i;
  }
  
  std::vector<std::vector<std::pair<int, int>>> adj_list(n+1);
  for (int i = 0; i < n; i++) {
    int u, v, l; std::cin >> u >> v >> l;
    adj_list[u].emplace_back(v, l);
  }
  
  VL sum_tunnels(n+1, 0);
  VL sum_rooms(n+1, 0);
  compute_stats(adj_list, sum_tunnels, sum_rooms, galleons, 0);
  long t = 0;
  std::cout << solve(adj_list, sum_tunnels, sum_rooms, galleons, 0, t) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}