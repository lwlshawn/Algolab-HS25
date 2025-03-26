#include <iostream>
#include <set>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

void solve() {
  int n, c, f; std::cin >> n >> c >> f;
  std::vector<std::set<std::string>> students(n); // vector[i] returns set of i's chars
  for (int i = 0; i < n; i++) {
    std::set<std::string> st;
    for (int j = 0; j < c; j++) {
      std::string trait; std::cin >> trait;
      st.insert(trait);
    }
    students[i] = st;
  }
  
  graph G(n);
  for (int i = 0; i < n; i++) {
    // commented lines cause TLE :) 
    // std::set<std::string> student_i = students[i];
    for (int j = i+1; j < n; j++) {
      // std::set<std::string> student_j = students[j];
      std::vector<std::string> intersection;
      std::set_intersection(students[i].begin(), students[i].end(), students[j].begin(), students[j].end(), std::back_inserter(intersection));
      if (intersection.size() > f) boost::add_edge(i, j, G);
    }
  }
  
  std::vector<vertex_desc> mate_map(n);  // exterior property map
  boost::edmonds_maximum_cardinality_matching(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));
  int matching_size = boost::matching_size(G,
    boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, G)));

  if (matching_size == n/2) std::cout << "not optimal" << std::endl;
  else std::cout << "optimal" << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}