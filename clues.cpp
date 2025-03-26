///2
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <queue>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

void bfs(int root, std::vector<std::vector<int>>& adj_list, std::vector<int>& colors, boost::disjoint_sets_with_storage<>& uf) {
  colors[root] = 0;
  std::queue<int> queue;
  queue.push(root);
  int curr_color = 0;
  
  while (!queue.empty()) {
    int curr = queue.front();
    int curr_color = colors[curr];
    queue.pop();

    for (int v : adj_list[curr]) {
      if (colors[v] == -1) {
        colors[v] = (curr_color + 1) % 2;
        queue.push(v);
      }
    }
  }
}

void testcase() {
  int n, m; long r; std::cin >> n >> m >> r;
  long sqr_rad = r * r;
  
  std::vector<IPoint> points;
  for (Index i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());

  EdgeV edges;
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  std::vector<std::vector<int>> adj_list(n);
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    int u = std::get<0>(*e);
    int v = std::get<1>(*e);
    
    Index c1 = uf.find_set(u);
    Index c2 = uf.find_set(v);
    K::FT curr_len = std::get<2>(*e);
    if (curr_len > sqr_rad) break;
    if (c1 != c2) {
      uf.link(c1, c2);
      adj_list[u].push_back(v);
      adj_list[v].push_back(u);
      if (--n_components == 1) break;
    }
  }
  
  //now i have all the edges of my forest, need to 2 color the components
  std::vector<int> colors(n, -1);
  for (int i = 0; i < n; i++) {
    if (colors[i] == -1) {
      bfs(i, adj_list, colors, uf);
    }
  }

  std::vector<IPoint> red_points;
  std::vector<IPoint> blue_points;
  for (int i = 0; i < n; i++) {
    if (colors[i] == 0) red_points.push_back(points[i]);
    else blue_points.push_back(points[i]);
  }
  
  Delaunay red_t;
  red_t.insert(red_points.begin(), red_points.end());
  
  Delaunay blue_t;
  blue_t.insert(blue_points.begin(), blue_points.end());
  
  bool feasible = true;
  for (auto e = red_t.finite_edges_begin(); e != red_t.finite_edges_end(); e++) {
    K::FT curr_len = t.segment(e).squared_length();
    if (curr_len <= sqr_rad) {
      feasible = false;
      break;
    }
  }
  
  for (auto e = blue_t.finite_edges_begin(); e != blue_t.finite_edges_end(); e++) {
    K::FT curr_len = t.segment(e).squared_length();
    if (curr_len <= sqr_rad) {
      feasible = false;
      break;
    }
  }
  
  //clues
  for (int i = 0; i < m; i++) {
    int a_x, a_y, b_x, b_y; std::cin >> a_x >> a_y >> b_x >> b_y;
    if (!feasible) std::cout << "n";
    else {
      K::Point_2 holmes(a_x, a_y);
      K::Point_2 watson(b_x, b_y);
      if (CGAL::squared_distance(holmes, watson) <= sqr_rad) {
        std::cout << "y";
      } else {
        auto vh1 = t.nearest_vertex(holmes);
        auto vh2 = t.nearest_vertex(watson);
        int u = vh1->info();
        int v = vh2->info();
        
        if (CGAL::squared_distance(holmes, vh1->point()) > sqr_rad || CGAL::squared_distance(watson, vh2->point()) > sqr_rad) {
          std::cout << "n";
        } else {
          if (uf.find_set(u) == uf.find_set(v)) {
            std::cout << "y";
          } else {
            std::cout << "n";
          }
        }
      }
    }
  }
  std::cout << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}