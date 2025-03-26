///3
// STL includes
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
  graph G(n);
  edge_adder adder(G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  for (int i = 0; i < m; i++) {
    int a, b, c; std::cin >> a >> b >> c;
    adder.add_edge(a, b, c);
    // adder.add_edge(b, a, c);
  }
  
  //10^3 * 5 * 10^3 doesn't fit in an int in worst case
  long min_cost = -1;
  for (int i = 1; i < n; i++) {
    //min-cut costs differ depending on which "part" you take
    long flow = boost::push_relabel_max_flow(G, 0, i);
    if (min_cost == -1 || flow < min_cost) min_cost = flow;
    
    flow = boost::push_relabel_max_flow(G, i, 0);
    if (min_cost == -1 || flow < min_cost) min_cost = flow;
  }
  std::cout << min_cost << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
