#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef  boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
  boost::property<boost::edge_capacity_t, long,
    boost::property<boost::edge_residual_capacity_t, long,
      boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >  graph;
// Interior Property Maps
typedef  boost::graph_traits<graph>::edge_descriptor      edge_desc;
typedef  boost::graph_traits<graph>::out_edge_iterator      out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int n, m; std::cin >> n >> m;
  
  const long INF = 100000000;
  graph G(n);
  edge_adder adder(G);
  const int v_source = boost::add_vertex(G);
  const int v_sink = boost::add_vertex(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  std::vector<int> conv;
  for (int i = 0; i < n; i++) {
    int s_i; std::cin >> s_i;
    conv.push_back(s_i);
    
    if (s_i > 0) {
      adder.add_edge(v_source, i, s_i);
    } else {
      adder.add_edge(i, v_sink, -s_i);
    }
  }
  
  for (int i = 0; i < m; i++) {
    int u, v; std::cin >> u >> v;
    adder.add_edge(u, v, INF);
  }

  // Find a min cut via maxflow
  boost::push_relabel_max_flow(G, v_source, v_sink);

  // BFS to find vertex set S
  std::vector<int> vis(n+2, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  vis[v_source] = true; // Mark the source as visited
  Q.push(v_source);
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }

  // Output S
  long sum_scores = 0;
  for (int i = 0; i < n; ++i) {
    if (vis[i]) sum_scores += conv[i];
  }
  
  if (sum_scores > 0) {
    std::cout << sum_scores << std::endl;
  } else {
    std::cout << "impossible\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}