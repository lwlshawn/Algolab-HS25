///1
#include <iostream>
#include <vector>
#include <tuple>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
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

typedef std::tuple<int, int, int, bool> Edge;

void testcase() {
  // std::cerr << "testcase begins\n";
  int n, m, s, p; std::cin >> n >> m >> s >> p;
  
  std::vector<std::vector<Edge>> adj_list(n);
  for (int i = 0; i < m; i++) {
    int u, v, c, d; std::cin >> u >> v >> c >> d;
    adj_list[u].push_back(Edge(v, c, d, false));
    adj_list[v].push_back(Edge(u, c, d, false));
  }
  
  //start from s, travel to p
  std::vector<long> dist(n, -1);
  std::priority_queue<std::pair<long, int>, std::vector<std::pair<long, int>>, std::greater<std::pair<long, int>>> pq;
  dist[s] = 0;
  pq.push(std::pair<long, int>(0, s));
  
  while (!pq.empty()) {
    std::pair<long, int> curr_pair = pq.top();
    pq.pop();
    int curr = curr_pair.second;
    long curr_dist = curr_pair.first;
    
    if (dist[curr] < curr_dist) continue;
    
    for (Edge e : adj_list[curr]) {
      int v = std::get<0>(e);
      int d = std::get<2>(e);
      if (dist[v] == -1 || dist[curr] + d < dist[v]) {
        pq.push(std::pair<long, int>(dist[curr] + d, v));
        dist[v] = dist[curr] + d;
      }
    }
  }
  
  // std::cerr << "printing distances after first dijkstra's" << std::endl;
  // for (int i = 0; i < n; i++) {
  //   std::cerr << dist[i] << ' ';
  // }
  // std::cerr << std::endl;
  
  
  graph G(n);
  edge_adder adder(G);
  pq.push(std::pair<long, int>(0, s));
  while (!pq.empty()) {
    std::pair<long, int> curr_pair = pq.top();
    pq.pop();
    int curr = curr_pair.second;
    long curr_dist = curr_pair.first;
    
    if (dist[curr] < curr_dist) continue;
    // std::cerr << "processing node: " << curr << std::endl;
    for (Edge& e : adj_list[curr]) {
      int v = std::get<0>(e);
      int c = std::get<1>(e);
      int d = std::get<2>(e);
      
      // std::cerr << "considering neighbour: " << v << std::endl;
      // std::cerr << "dist[curr] dist[v]: " << dist[curr] << ' ' << dist[v] << std::endl;
      // std::cerr << "and edge length is: " << d << std::endl;
      // std::cerr << "edge used?: " << std::get<3>(e) << std::endl;
      
      if (dist[curr] + d == dist[v] && !std::get<3>(e)) {
        std::get<3>(e) = true;
        // std::cerr << "adding edge from: " << curr << " to " << v << " with cap: " << c << std::endl;
        adder.add_edge(curr, v, c);
        pq.push(std::pair<long, int>(dist[v], v));
      }
    }
  }
  
  long flow = boost::push_relabel_max_flow(G, s, p);
  std::cout << flow << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}

