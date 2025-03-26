#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

#include <iostream>
#include <vector>
#include <algorithm>

typedef boost::adjacency_list<boost::vecS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::no_property,
                              boost::property<boost::edge_weight_t, int>> weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;

void kruskal(const weighted_graph &G, const weight_map &weights) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)

  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int sum = 0;
  for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it) {
    sum += weights[*it];
  }
  std::cout << sum << ' ';
}

int dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int max = -1;
  for (int dist : dist_map) {
    max = std::max(dist, max);
  }
  return max;
}

void solve() {
  int n, m; std::cin >> n >> m;
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  edge_desc e;
  for (int i = 0; i < m; i++) {
    int u, v, w; std::cin >> u >> v >> w;
    e = boost::add_edge(u, v, G).first; weights[e] = w;
  }
  
  kruskal(G, weights);
  std::cout << dijkstra_dist(G, 0) << std::endl;;
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}