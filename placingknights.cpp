///2
#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <unordered_set>

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


std::vector<std::pair<int, int>> valid_indices(int i, int j, int n) {
  std::vector<std::pair<int, int>> res;
  if (i - 1 >= 0 and j - 2 >= 0) res.emplace_back(i-1, j-2);
  if (i - 1 >= 0 and j + 2 < n) res.emplace_back(i-1, j+2);
  if (i + 1 < n and j - 2 >= 0) res.emplace_back(i+1, j-2);
  if (i + 1 < n and j + 2 < n) res.emplace_back(i+1, j+2);
  if (i - 2 >= 0 and j - 1 >= 0) res.emplace_back(i-2, j-1);
  if (i - 2 >= 0 and j + 1 < n) res.emplace_back(i-2, j+1);
  if (i + 2 < n and j - 1 >= 0) res.emplace_back(i+2, j-1);
  if (i + 2 < n and j + 1 < n) res.emplace_back(i+2, j+1);
  return res;
}

void testcase() {
  // std::cerr << "testcase begins" << std::endl;
  int n; std::cin >> n;
  std::vector<std::vector<int>> present(n, std::vector<int>(n, -1));
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      bool pres; std::cin >> pres;
      present[i][j] = pres;
    }
  }
  
  graph G(n*n);
  edge_adder adder(G);
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (!present[i][j]) continue;
      
      if ((i + j) % 2 == 0) {
        adder.add_edge(v_source, i*n + j, 1);
        for (std::pair<int, int> indices : valid_indices(i,j,n)) {
          if (present[indices.first][indices.second]) {
            adder.add_edge(i*n + j, indices.first*n + indices.second, n*n + 10);
          }
        }
      } else {
        adder.add_edge(i*n + j, v_sink, 1);
      }
    }
  }
  
  std::vector<std::unordered_set<int>> G_new(n*n + 2, std::unordered_set<int>());
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  // std::cerr << "max matching has size: " << flow << std::endl;
  const auto c_map = boost::get(boost::edge_capacity, G);
  const auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto edge_iters = boost::edges(G);
  for (auto edge_it = edge_iters.first; edge_it != edge_iters.second; ++edge_it) {
    const edge_desc edge = *edge_it;
    if (c_map[edge] - rc_map[edge] > 0) { //if flow through edge > 0
      G_new[boost::target(edge, G)].insert(boost::source(edge, G));
    }
    if (rc_map[edge] > 0) { //if flow less than capacity
      G_new[boost::source(edge, G)].insert(boost::target(edge, G));
    }
  }
  
  //i manually did the bfs on the residual graph i have no idea why
  std::vector<bool> visited(n*n + 2, false);
  std::queue<int> q;
  q.push(n*n);
  while (!q.empty()) {
    int curr = q.front();
    q.pop();
    visited[curr] = true;
    for (int nbr : G_new[curr]) {
      if (!visited[nbr]) {
        q.push(nbr);
      }
    }
  }
  
  int count = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (!present[i][j]) continue;
      
      if ((i + j) % 2 == 0) {
        if (visited[i*n + j]) {
          // std::cerr << "counting even " << i*n + j << std::endl;
          count++;
        }
      } else {
        if (!visited[i*n + j]) {
          // std::cerr << "counting odd " << i*n + j << std::endl;
          count++;
        }
      }
    }
  }
  
  std::cout << count << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
