///4
#include <iostream>
#include <vector>
#include <map>
#include <set>

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
  int N, S; std::cin >> N >> S;
  graph G(S);
  edge_adder adder(G);  

  const int v_source = boost::add_vertex(G);
  const int v_target = boost::add_vertex(G); 
  const long INF = 10000000; //total profit is at most 100 * 10*4 = 10*6. 10*7 for INF should suffice
  //idea is to mod out the INF of the final cost at the end

  for (int i = 0; i < S; i++) {
    int l_i; std::cin >> l_i;
    adder.add_edge(v_source, i, l_i, 0);
  }

  std::vector<std::map<int, int>> vertex_maps(S); //vertex_maps[s_i][t] gives index of (s_i, t)
  for (int i = 0; i < S; i++) {
    vertex_maps[i][0] = i; //the first i vertices of the graph we designate to be (s_i, 0)
  }
  
  int max_time = -1;
  for (int i = 0; i < N; i++) {
    int s_i, t_i, d_i, a_i, p_i;
    std::cin >> s_i >> t_i >> d_i >> a_i >> p_i;
    
    int v1;
    if (vertex_maps[s_i - 1].count(d_i) == 0) {
      v1 = boost::add_vertex(G);
      vertex_maps[s_i - 1][d_i] = v1;
    } else {
      v1 = vertex_maps[s_i - 1][d_i];
    }
    
    int v2;
    if (vertex_maps[t_i - 1].count(a_i) == 0) {
      v2 = boost::add_vertex(G);
      vertex_maps[t_i-1][a_i] = v2;
    } else {
      v2 = vertex_maps[t_i-1][a_i];
    }

    max_time = std::max(max_time, a_i);
    adder.add_edge(v1, v2, 1, INF * (a_i - d_i) - p_i);
  }
  
  for (int i = 0; i < S; i++) {
    for (auto it = vertex_maps[i].begin(); std::next(it) != vertex_maps[i].end(); it++) {
      int t1 = it->first;
      int v1 = it->second;
      
      int t2 = std::next(it)->first;
      int v2 = std::next(it)->second;
      
      adder.add_edge(v1, v2, INF, INF * (t2 - t1));
    }
    
    auto last = std::prev(vertex_maps[i].end());
    int t = last->first;
    int v = last->second;
    adder.add_edge(v, v_target, INF, INF * (max_time - t));
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  long cost2 = boost::find_flow_cost(G);
  cost2 = (cost2 % INF) - INF;
  
  if (cost2 == -INF) { //this WAS the last hidden edgecase
    std::cout << 0 << std::endl;
  } else {  
    std::cout << -cost2 << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
