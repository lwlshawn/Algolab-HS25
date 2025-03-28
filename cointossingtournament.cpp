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
  int n, m; std::cin >> n >> m;
  graph G(n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);

  std::vector<int> scores(n, 0);
  int count = 0;
  for (int i = 0; i < m; i++) {
    int a, b, c; std::cin >> a >> b >> c;
    if (c == 1) {
      scores[a] += 1;
      count += 1;
    } else if (c == 2) {
      scores[b] += 1;
      count += 1;
    } else {
      vertex_desc g = boost::add_vertex(G);
      adder.add_edge(v_source, g, 1);
      adder.add_edge(g, a, 1);
      adder.add_edge(g, b, 1);
    }
    
  }
  
  long sum = 0;
  bool terminate = false;
  for (int i = 0; i < n; i++) {
    int s; std::cin >> s;
    if (scores[i] > s) {
      // std::cerr << "terminating" << std::endl;
      terminate = true;
    }
    adder.add_edge(i, v_sink, s - scores[i]);
    sum += s - scores[i];
  }

  if ((m - count) != sum || terminate) {
    // std::cerr << "m is: " << m << std::endl;
    // std::cerr << "sum is: " << sum << std::endl;
    std::cout << "no" << std::endl;
    return;
  }
  
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