///1
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
  int n; std::cin >> n;
  graph G(n+2); //day nodes 0 to n-1, source n, sink n+1
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  for (int i = 0; i < n; i++) {
    int a_i, c_i; std::cin >> a_i >> c_i;
    adder.add_edge(n, i, a_i, c_i);
  }
  
  int max_price = 0;
  int count_students = 0;
  std::vector<int> students, prices;
  for (int i = 0; i < n; i++) {
    int s_i, p_i; std::cin >> s_i >> p_i;
    if (p_i > max_price) max_price = p_i;
    count_students += s_i;
    
    students.push_back(s_i);
    prices.push_back(p_i);
  }
  
  for (int i = 0; i < n; i++) {
    //instead of giving the "cost" "-price", scale to "max_price - price" for non-negativity
    adder.add_edge(i, n+1, students[i], max_price - prices[i]); //only way to reach the sink
  }
  
  for (int i = 0; i < n-1; i++) {
    int v_i, e_i; std::cin >> v_i >> e_i;
    adder.add_edge(i, i+1, v_i, e_i);
  }

  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, n, n+1);
  int cost2 = boost::find_flow_cost(G);
  
  // Or equivalently, you can do the summation at the sink, but with reversed edge.
  int t_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(n+1,G), G); e != eend; ++e) {
      t_flow += rc_map[*e] - c_map[*e];  
  }
  // t_flow represents number of meals we managed to serve
  if (t_flow == count_students) {
    std::cout << "possible ";
  } else {
    std::cout << "impossible ";
  }
  
  std::cout << t_flow << ' ' << -(cost2 - (t_flow * max_price)) << std::endl;
}



int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}