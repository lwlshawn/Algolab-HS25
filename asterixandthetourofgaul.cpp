#include <iostream>
#include <vector>
#include <algorithm>

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
  int n, m; std::cin >> n >> m;
  
  graph G(n);
  edge_adder adder(G);
  const int v_source = boost::add_vertex(G);
  const int v_sink = boost::add_vertex(G);
  const long INF = 100000000;
  
  std::vector<int> caps;
  for (int i = 0; i < n-1; i++) {
    int c_i; std::cin >> c_i;
    caps.push_back(c_i);
    
    adder.add_edge(v_source, i, c_i, INF);
    adder.add_edge(i, i+1, c_i, INF);
    adder.add_edge(i+1, v_sink, c_i, INF);
  }
  
  
  std::vector<std::vector<std::vector<int>>> foods(n, std::vector<std::vector<int>>(n));
  for (int i = 0; i < m; i++) {
    int a, b, d; std::cin >> a >> b >> d;
    foods[a][b].push_back(d);
  }
  
  for (int i = 0; i < n-1; i++) {
    for (int j = i+1; j < n; j++) {
      //for each pair i < j
      int min_cap = caps[i]; 
      //say i = 2, j = 4, so need caps (2,3) (3,4) which are entries
      //caps[i] caps[i+1]... caps[j-1]
      for (int k = i+1; k < j; k++) {
        min_cap = std::min(caps[k], min_cap);
      }
        
      if (foods[i][j].size() < min_cap) {
        for (int d : foods[i][j]) {
          adder.add_edge(i, j, 1, (j - i) * INF - d);
        }
      } else {
        std::sort(foods[i][j].begin(), foods[i][j].end(), std::greater<int>());
        for (int k = 0; k < min_cap; k++) {
          adder.add_edge(i, j, 1, (j - i) * INF - foods[i][j][k]);
        }
        
      }
    }
  }

  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  long cost2 = boost::find_flow_cost(G);
  std::cout << -((cost2 % INF) - INF) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}