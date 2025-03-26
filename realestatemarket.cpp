// ALGOLAB BGL Tutorial 3
// Code demonstrating 
// - MinCostMaxFlow with arbitrary edge costs using cycle_canceling
// - MinCostMaxFlow with non-negative edge costs using successive_shortest_path_nonnegative_weights

// Includes
// ========
#include <iostream>
#include <vector>

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
typedef traits::vertex_descriptor vertex_desc;



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
  // std::cerr << "testcase begins" << std::endl;
  int n, m, s; std::cin >> n >> m >> s;
  graph G(n + m + s); 
  //buyers are 0 ... n-1
  //pieces of land n ... n+m-1
  //states are n+m ... n+m+s-1
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  //edges from states to sink
  for (int i = 0; i < s; i++) { //for each state
    int l_i; std::cin >> l_i; //limit on the number of cities in state
    adder.add_edge(n+m+i, v_sink, l_i, 0);
  }
  
  //edges from land to states
  for (int i = 0; i < m; i++) { //for each piece of land
    int s_i; std::cin >> s_i; //state the land begins in
    adder.add_edge(n+i, n+m+s_i-1, 1, 0);
  }
  
  int maximum_bid = 0;
  std::vector<std::vector<int>> bids;
  for (int i = 0; i < n; i++) { //for each buyer
    std::vector<int> buyer;
    for (int j = 0; j < m; j++) { //for each of piece of land
      int b_i; std::cin >> b_i;
      buyer.push_back(b_i);
      if (b_i > maximum_bid) maximum_bid = b_i;
    }
    bids.push_back(buyer);
  }
  
  //edges from source to buyers
  for (int i = 0; i < n; i++) {
    adder.add_edge(v_source, i, 1, 0);
    //edges from buyers to land
    for (int j = 0; j < m; j++) {
      adder.add_edge(i, n+j, 1, maximum_bid - bids[i][j]);
    }
  }
  
  // Run the algorithm
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int cost2 = boost::find_flow_cost(G);
  // Iterate over all edges leaving the source to sum up the flow values.
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      // std::cerr << "edge from " << boost::target(*e, G) << " to " << boost::source(*e, G) 
      //     << " with capacity " << c_map[r_map[*e]] << " and residual capacity " << rc_map[r_map[*e]] << "\n";

      s_flow += c_map[*e] - rc_map[*e];     
  }
  std::cout << s_flow << ' ' << -(cost2 - (maximum_bid * s_flow)) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
