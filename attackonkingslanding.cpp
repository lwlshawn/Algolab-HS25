///2
#include <iostream>
#include <vector>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void maximum_matching(const graph &G) {
  int n = boost::num_vertices(G);
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  const vertex_desc NULL_VERTEX = boost::graph_traits<graph>::null_vertex();

  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  std::cout << matching_size << std::endl;
}

void testcase() {
  //idea: connect a vertex s to all the barracks with dist=0 and do a single dijkstra's from s
  //delete all vertices with distance > d, as they can't be reached by any barracks
  //remaining vertices are all reachable from barracks
  
  //problem now reduces to finding a maximum matching. Plazas should reduce to simply
  //duplicating the relevant vertex with the same outgoing edges
  int n, m, b, p, d; std::cin >> n >> m >> b >> p >> d;
  
  std::vector<int> barracks(b);
  for (int i = 0; i < b; i++) {
    int a_i; std::cin >> a_i;
    barracks[i] = a_i;
  }
  
  std::vector<bool> is_plaza(n, false);
  for (int i = 0; i < p; i++) {
    int q_i; std::cin >> q_i;
    is_plaza[q_i] = true;
  }
  
  std::vector<std::vector<std::pair<int, int>>> adj_list(n);
  for (int i = 0; i < m; i++) {
    int x, y, l; std::cin >> x >> y >> l;
    adj_list[x].emplace_back(y, l);
    adj_list[y].emplace_back(x, l);
  }
  
  std::priority_queue<std::pair<long, int>, std::vector<std::pair<long, int>>, std::greater<std::pair<long, int>>> pq;
  std::vector<long> dist(n, -1);
  for (int b : barracks) {
    dist[b] = 0;
    pq.push(std::pair<long, int>(0, b));
  }
  
  while (!pq.empty()) {
    std::pair<long, int> p = pq.top();
    pq.pop();
    
    int curr_v = p.second;
    int curr_d = p.first;
    if (curr_d > dist[curr_v]) continue;
    
    for (auto p : adj_list[curr_v]) {
      int nbr = p.first;
      int l = p.second;
      if (dist[nbr] == -1 || dist[curr_v] + l < dist[nbr]) {
        dist[nbr] = dist[curr_v] + l;
        pq.push(std::pair<long, int>(dist[nbr], nbr));
      }
    }
  }
  
  //now build a new graph on which we do maximum matching.
  //now how to account for plazas? if a plaza is surrounded by non-plazas,
  //i just copy the vertex and copy the edges of the original vertex
  
  //but what about two plazas connected ot each other? does this model still work?
  //yes i think so actually
  graph G(2*n); //second set of n vertices are dummies just for plaza insertion
  for (int i = 0; i < n; i++) {
    if (dist[i] <= d) {
      for (auto p : adj_list[i]) {
        int nbr = p.first;
        if (dist[nbr] <= d) {
          boost::add_edge(i, nbr, G);
          
          if (is_plaza[i]) {
            boost::add_edge(n+i, nbr, G);
          }
        }
      }
    }
  }
  maximum_matching(G);

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}