///2
// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

void testcase() {
  int l, p; std::cin >> l >> p;
  graph G(l); // 0 up to l-1 are locations, l is source, l+1 is target
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  edge_adder adder(G);

  long sum = 0;
  for (int i = 0; i < l; i++) {
    int g, d; std::cin >> g >> d;
    adder.add_edge(v_source, i, g);
    adder.add_edge(i, v_sink, d);
    sum += d;
  }
  
  // Retrieve the capacity map and reverse capacity map
  const auto c_map = boost::get(boost::edge_capacity, G);
  for (int i = 0; i < p; i++) {
    int f, t, c, C; std::cin >> f >> t >> c >> C;
    
    //place an edge with capacity C - c
    adder.add_edge(f, t, C - c);
    
    //increase the demand of the source
    edge_desc e = boost::edge(f, v_sink, G).first;
    c_map[e] += c;
    
    //increase the supply to sink
    e = boost::edge(v_source, t, G).first;
    c_map[e] += c;
    
    sum += c;
  }
  // Calculate flow from source to sink
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if (flow == sum) {
    std::cout << "yes" << std::endl;
  } else {
    std::cout << "no" << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
