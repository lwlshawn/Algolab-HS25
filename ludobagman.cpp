///1
#include <iostream>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};


void testcase() {
  int e, w, m, d; std::cin >> e >> w >> m >> d;
  int p, l; std::cin >> p >> l;
  
  // Create graph, edge adder class and propery maps
  graph G(2*(e + w));
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto w_map = boost::get(boost::edge_weight, G);
  
  const int v_source = boost::add_vertex(G);
  const int cap_node = boost::add_vertex(G);
  const int v_target = boost::add_vertex(G);
  const long INF = 100000000;

  adder.add_edge(v_source, cap_node, p, 0);
  for (int i = 0; i < e; i++) {
    adder.add_edge(cap_node, i, l, 0);
    adder.add_edge(cap_node, i, w, INF);
    
    adder.add_edge(cap_node, e + w + i, w, INF);
  }
  
  for (int i = 0; i < w; i++) {
    adder.add_edge(e + i, v_target, l, 0);
    adder.add_edge(e + i, v_target, e, INF);
    
    adder.add_edge(e + w + e + i, v_target, e, INF);
  }
  
  for (int i = 0; i < m; i++) {
    int u, v, r; std::cin >> u >> v >> r;
    adder.add_edge(u, e + v, 1, r);
  }
  
  for (int i = 0; i < d; i++) {
    int u, v, r; std::cin >> u >> v >> r;
    adder.add_edge(e + w + u, 2*e + w + v, 1, r);
  }
  
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  long cost2 = boost::find_flow_cost(G);

  // Iterate over all edges leaving the source to sum up the flow values.
  // long s_flow = 0;
  out_edge_it edge, eend;
  for(boost::tie(edge, eend) = boost::out_edges(boost::vertex(cap_node , G), G); edge != eend; ++edge) {
    long edge_flow = c_map[*edge] - rc_map[*edge];
    if (w_map[*edge] == 0 && boost::target(*edge, G) != v_source) {
      //edge from cap_node to east team
      if (edge_flow != l) {
        std::cout << "No schedule!" << std::endl;
        return;
      }
    } else if (w_map[*edge] == INF) {
      cost2 -= edge_flow * INF;
    }
  }

  for(boost::tie(edge, eend) = boost::out_edges(boost::vertex(v_target,G), G); edge != eend; ++edge) {
    long edge_flow = rc_map[*edge] - c_map[*edge];
    if (w_map[*edge] == 0) {
      if (edge_flow != l) {
        std::cout << "No schedule!" << std::endl;
        return;
      }
    } else if (w_map[*edge] == -INF) {
      cost2 -= (edge_flow * INF);
    }
  }
  
  std::cout << cost2 << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
