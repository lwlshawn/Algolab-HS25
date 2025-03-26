#include <iostream>
#include <vector>

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;
typedef std::vector<long> VL;
typedef std::vector<VL> VVL;

void solve(int root, VVI& adj_list, VI& costs, VVL& DP) {
  for (int child : adj_list[root]) {
    solve(child, adj_list, costs, DP);
  }
  
  if (adj_list[root].size() == 0) {
    DP[root][0] = 0;
    DP[root][1] = costs[root];
    DP[root][2] = costs[root];
  } else {
    long sum_childs_0 = 0, sum_childs_1 = 0, sum_childs_2 = 0;
    for (int child : adj_list[root]) {
      sum_childs_0 += DP[child][0];
      sum_childs_1 += DP[child][1];
      sum_childs_2 += DP[child][2];
    }
    
    //if i definitely repaired, then for each child, take the mincost with them left possibly uncovered
    DP[root][1] = costs[root] + sum_childs_0;
    
    //i can be left uncovered : best might be a solution that repairs i anyway, else if we dont repair i, we just need
    //the childrens subtrees to be covered as cheaply as possible
    DP[root][0] = std::min(sum_childs_2, DP[root][1]);
    
    //if the full subtree is repaired, either we repaired the root (DP[root][1]), OR we repaired one of its children
    //if we repair a specific child, we just need the other children to be covered
    DP[root][2] = DP[root][1];
    for (int child : adj_list[root]) {
      DP[root][2] = std::min(DP[root][2], DP[child][1] + sum_childs_2 - DP[child][2]);
    }
    
    //DP[i][0] - mincost of repairing the subtree rooted at i, among all solutions where i can be left uncovered
    //DP[i][1] - mincost of repairing the subtree rooted at i, among all solutions where i is repaired
    //DP[i][2] - mincost of repairing the subtree rooted at i 

  }
  


  
}

void testcase() {
  int n; std::cin >> n;
  std::vector<std::vector<int>> adj_list(n);
  for (int i = 0; i < n-1; i++) {
    int u, v; std::cin >> u >> v;
    adj_list[u].push_back(v);
  }
  
  std::vector<int> costs;
  for (int i = 0; i < n; i++) {
    int c_i; std::cin >> c_i;
    costs.push_back(c_i);
  }
  
  std::vector<std::vector<long>> DP(n, std::vector<long>(3, -1));
  solve(0, adj_list, costs, DP);
  std::cout << DP[0][2] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}