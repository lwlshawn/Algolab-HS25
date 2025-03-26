// STL includes
#include <iostream>
#include <vector>
#include <utility>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>


typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

std::vector<edge_desc> kruskal(const weighted_graph &G) {
  std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  return mst;
}

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

void solve() {
  // for each species we want to find the MST
  // combine all the MSTs into one graph
  // use dijkstras from node a to node b.
  int n, m, s, a, b; std::cin >> n >> m >> s >> a >> b;
  std::vector<weighted_graph> all_graphs(s, weighted_graph(n));
  std::vector<weight_map> all_weights(s);
  
  for (int i = 0; i < s; i++) {
    all_weights[i] = boost::get(boost::edge_weight, all_graphs[i]);
  }
  
  edge_desc edge;
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    for (int j = 0; j < s; j++) {
      int ws; std::cin >> ws;
      edge = boost::add_edge(u, v, all_graphs[j]).first;
      all_weights[j][edge] = ws;
    }
  }
  
  // hive values are meaningless as the MSTs are unique
  for (int i = 0; i < s; i++) {
    int h; std::cin >> h;
  }
  
  weighted_graph G(n);
  weight_map weights = boost::get(boost::edge_weight, G);
  
  for (int i = 0; i < s; i++) {
    // for each species, compute the unique MST
    std::vector<edge_desc> mst = kruskal(all_graphs[i]);
    for (edge_desc tree_edge : mst) {
      // now for each edge of the MST, add it to the tree if it improves
      int u = boost::source(tree_edge, all_graphs[i]);
      int v = boost::target(tree_edge, all_graphs[i]);
      int w = all_weights[i][tree_edge];
      
      if (!boost::edge(u, v, G).second) {
        // edge does not exist
        edge_desc new_edge = boost::add_edge(u, v, G).first;
        weights[new_edge] = w;      
      } else {
        // add it only if the edge improves
        edge_desc curr_edge = boost::edge(u, v, G).first;
        if (w < weights[curr_edge]) {
          weights[curr_edge] = w;
        }
      }
    }
  }
  
  std::cout << dijkstra_dist(G, a, b) << std::endl;
  



}

int main() {
  int t; std::cin >> t; 
  for (int i = 0; i < t; i++) {
    solve();
  }
}
