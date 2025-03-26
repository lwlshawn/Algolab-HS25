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
  int m, n, k, C; std::cin >> m >> n >> k >> C;
  if (m == 0 || n == 0) {
    std::cout << 0 << std::endl;
    return;
  }
  
  graph G(2 * m * n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  //instead of m * n vertices, we have 2 * m * n by splitting
  //every vertex into two vertices. an in vertex, and an out vertex
  //there are n rows, m columns
  
  //normally the indexing would be i * n + m
  //now its 2 * (i * n + m)
  // 2 * (i * n + m) + 1
  
  // build the graph edges
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      //looking at intersection at location (i, j)
      //represented by nodes 2*(i*m + j), 2*(i*m + j) + 1
      //2*(i*m + j) is "in" node, 2*(i*m + j) + 1 is "out" node
      //edge between them enforces capacity C
      adder.add_edge(2*(i*m + j), 2*(i*m + j) + 1, C);
      
      //connect intersection to nearby valid intersections, out to in
      //capacity 1 to represent segment collapsing after single knight
      if (i > 0) adder.add_edge(2*(i*m + j) + 1, 2*((i-1)*m + j), 1);
      if (i + 1 < n) adder.add_edge(2*(i*m + j) + 1, 2*((i+1)*m + j), 1);
      if (j > 0) adder.add_edge(2*(i*m + j) + 1, 2*(i*m + j - 1), 1);
      if (j + 1 ) adder.add_edge(2*(i*m + j) + 1, 2*(i*m + j + 1), 1);
    }
  }
  
  //we should also connect all the corner nodes to the target
  for (int i = 0; i < n; i++) {
    if (i == 0 || i == n - 1) {
      //these intersections are intersections {0,...,m-1} and {i*m,...,i*m + m-1}
      if (m != 0) adder.add_edge(2*i*m + 1, 2*m*n + 1, 2);
      if (m != 1) adder.add_edge(2*(i*m + m-1) + 1, 2*m*n + 1, 2);
      
      for (int j = 1; j < m-1; j++) {
        adder.add_edge(2*(i*m + j) + 1, 2*m*n + 1, 1);
      }
      
    } else {
      // first and last intersection on row i
      // so they are intersections i*m and i*m + (m-1)
      // and their out vertices have indices 2*(i*m) + 1, 2*(i*m + (m-1)) + 1
      if (m != 0) adder.add_edge(2*i*m + 1, 2*m*n + 1, 1); 
      if (m != 1) adder.add_edge(2*(i*m + m-1) + 1, 2*m*n + 1, 1);
    }
  }
  
  
  //finally we add a source link to each knight location
  for (int i = 0; i < k; i++) {
    int x, y; std::cin >> y >> x;
    adder.add_edge(v_source, 2*(x*m + y), 1);
  }
  
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}