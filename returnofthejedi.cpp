#include <iostream>
#include <vector>
#include <tuple>
#include <queue>
#include <algorithm>
#include <boost/pending/disjoint_sets.hpp>

typedef std::tuple<int, int, int, bool> T;

void bfs(int root, std::vector<std::vector<std::pair<int, int>>>& tree_edges, std::vector<std::vector<int>>& dist, int n) {
  std::vector<bool> visited(n, false);
  visited[root] = true;
  
  std::queue<int> queue;
  queue.push(root);
  dist[root][root] = 0;

  while (!queue.empty()) {
    int curr = queue.front();
    queue.pop();
    
    for (auto p : tree_edges[curr]) {
      int v = p.first;
      if (visited[v]) continue;
      
      int c = p.second;
      dist[root][v] = std::max(dist[root][curr], c);
      visited[v] = true;
      
      queue.push(v);
    }
  }
}


void testcase() {
  int n, tatt; std::cin >> n >> tatt;
  tatt--;
  
  std::vector<T> edges;
  std::vector<std::vector<int>> adj_list(n, std::vector<int>(n, -1));
  for (int i = 1; i <= n-1; i++) { //jth line from 1,...,n-1
    for (int j = 0; j < n-i; j++) {//contains n-j integers 
      int c; std::cin >> c;
      adj_list[i-1][i-1 + j + 1] = c;
      adj_list[i-1 + j + 1][i-1] = c;
      
      edges.emplace_back(c, i-1, i-1 + j + 1, false);
    }
  }
  std::sort(edges.begin(), edges.end());
  boost::disjoint_sets_with_storage<> uf(n);
  
  int num_components = n;
  long tree_cost = 0;
  std::vector<std::vector<std::pair<int, int>>> tree_edges(n);
  for (int i = 0; i < edges.size(); i++) {
    T curr_edge = edges[i];
    int c = std::get<0>(curr_edge);
    int u = std::get<1>(curr_edge);
    int v = std::get<2>(curr_edge);

    int c1 = uf.find_set(u);
    int c2 = uf.find_set(v);
    if (c1 != c2) {
      uf.link(c1, c2);
      num_components--;
      std::get<3>(edges[i]) = true;
      tree_cost += c;
      
      tree_edges[u].emplace_back(v, c);
      tree_edges[v].emplace_back(u, c);
    }
    
    if (num_components == 1) {
      break;
    }
  }
  
  std::vector<std::vector<int>> dist(n, std::vector<int>(n, -1));
  for (int i = 0; i < n; i++) {
    bfs(i, tree_edges, dist, n);
  }
  
  int best = -1;
  for (T edge : edges) {
    if (!std::get<3>(edge)) {
      //edge not used
      int c = std::get<0>(edge);
      int u = std::get<1>(edge);
      int v = std::get<2>(edge);
      
      //dist[u][v] is the max edge between u and v in the bfs tree
      int cost = c - dist[u][v];
      if (best == -1 || cost < best) {
        best = cost;
      }
    }
  }
  
  std::cout << tree_cost + best << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}