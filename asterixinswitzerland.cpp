///4
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

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
  graph G(n);
  edge_adder adder(G);
  const int src = boost::add_vertex(G);
  const int sink = boost::add_vertex(G);
  
  std::vector<int> balances;
  for (int i = 0; i < n; i++) {
    int b_i; std::cin >> b_i;
    balances.push_back(b_i);
    
    if (b_i > 0) adder.add_edge(src, i, b_i);
    else adder.add_edge(i, sink, -b_i);
  }
  
  std::vector<std::vector<int>> adj_list(n, std::vector<int>(n, -1));
  for (int i = 0; i < m; i++) {
    int u, v, d_ij; std::cin >> u >> v >> d_ij;
    adder.add_edge(u, v, d_ij);
    adj_list[u][v] = d_ij;
  }
  
  // Find a min cut via maxflow
  long flow = boost::push_relabel_max_flow(G, src, sink);
  // std::cerr << "maximum flow = minimum cut = " << flow << "\n";
  auto rc_map = boost::get(boost::edge_residual_capacity, G);

  // BFS to find vertex set S
  std::vector<int> vis(n+2, false); // visited flags
  std::queue<int> Q; // BFS queue (from std:: not boost::)
  // std::cerr << "src is: " << src << std::endl;
  vis[src] = true; // Mark the source as visited
  Q.push(src);
  
  long sum_balances = 0;
  while (!Q.empty()) {
    const int u = Q.front();
    Q.pop();
    
    if (u < n) sum_balances += balances[u];
    
    out_edge_it ebeg, eend;
    for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
      const int v = boost::target(*ebeg, G);
      // Only follow edges with spare capacity
      if (rc_map[*ebeg] == 0 || vis[v]) continue;
      vis[v] = true;
      Q.push(v);
    }
  }
  
  long sum_outgoing = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (vis[i] && !vis[j]) sum_outgoing += adj_list[i][j];
    }
  }
  
  if (sum_balances > sum_outgoing) {
    std::cout << "yes\n";
  } else {
    std::cout << "no\n";
  }

  // Output S
  // for (int i = 0; i < n+2; ++i) {
  //   if (vis[i]) std::cerr << i << " ";
  // }
  // std::cerr << "\n";
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}