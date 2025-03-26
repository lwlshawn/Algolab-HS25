///1
// Algolab BGL Tutorial 2 (Max flow, by taubnert@ethz.ch)
// Flow example demonstrating how to use push_relabel_max_flow using a custom edge adder
// to manage the interior graph properties required for flow algorithms
#include <iostream>
#include <vector>
#include <algorithm>

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
  int w, h; std::cin >> w >> h;
  graph G(w*h);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  std::vector<std::vector<char>> grid(h, std::vector<char>(w, 'a'));
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      char c; std::cin >> c;
      grid[i][j] = c;
    }
  }
  
  // parity of sum of coordinates should work i think
  // (0, 0) (1, 0)
  // (1, 0) (1, 1)
  
  long count_even = 0;
  long count_odd = 0;
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      if (grid[i][j] == '.') {
        if ((i + j) % 2 == 0) {
          count_even += 1;
          // add an edge from the source
          adder.add_edge(v_source, i*w + j, 1);

          // add a tile to adjacent odd tiles
          if (i - 1 >= 0 && grid[i-1][j] == '.') {
            adder.add_edge(i*w + j, (i-1)*w + j, 1);
          }
          
          if (i + 1 < h && grid[i+1][j] == '.') {
            adder.add_edge(i*w + j, (i+1)*w + j, 1);
          }
          
          if (j - 1 >= 0 && grid[i][j-1] == '.') {
            adder.add_edge(i*w + j, i*w + j - 1, 1);
          }
          
          if (j + 1 < w && grid[i][j+1] == '.') {
            adder.add_edge(i*w + j, i*w + j + 1, 1);
          }
          
        } else {
          count_odd += 1;
          // add an edge to the sink
          adder.add_edge(i*w + j, v_sink, 1);

        }
      } else if (grid[i][j] == 'x') {
        continue;
      }
    }
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  if (flow == std::max(count_even, count_odd)) {
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

