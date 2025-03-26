///3
#include <iostream>
#include <map>
#include <algorithm>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase() {
  // std::cerr << "testcase begins\n";
  int n, h; K::FT s; std::cin >> n >> s >> h;
  std::vector<K::Point_2> pts;
  for (int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    pts.emplace_back(x,y);
  }
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  K::FT min_edge_len = -1;
  int a_2 = 0;
  for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) {
    K::FT curr_len = t.segment(it).squared_length();
    if (min_edge_len == -1 || curr_len < min_edge_len) {
      min_edge_len = curr_len;
      a_2 = 1;
    } else if (curr_len == min_edge_len) {
      a_2++;
    }
  }
  
  std::map<K::FT, int> counts;
  int a_3 = 0;
  K::FT min_face_len = -1;
  for (auto it = t.finite_faces_begin(); it != t.finite_faces_end(); it++) {
    auto v1 = it->vertex(0);
    auto v2 = it->vertex(1);
    auto v3 = it->vertex(2);
    
    K::Point_2 p1 = v1->point();
    K::Point_2 p2 = v2->point();
    K::Point_2 p3 = v3->point();
    K::Point_2 p4 = CGAL::circumcenter(p1, p2, p3);
    K::FT curr_len = CGAL::squared_distance(p1, p4);
    
    if (counts.count(curr_len) == 0) {
      counts[curr_len] = 1;
    } else {
      counts[curr_len]++;
    }
    
    if (min_face_len == -1 || curr_len < min_face_len) {
      min_face_len = curr_len;
      a_3 = 1;
    } else if (curr_len == min_face_len) {
      a_3++;
    }
  }
  
  
  std::vector<std::pair<K::FT, int>> bounds;
  int a_s = 0;
  for (auto it = t.finite_edges_begin(); it != t.finite_edges_end(); it++) {
    auto obj = t.dual(it);
    K::FT curr_edge_len = t.segment(it).squared_length();
    
    auto vh = it->first->vertex(it->first->ccw(it->second));
    K::Point_2 v = vh->point();

    if (const K::Segment_2 * seg = CGAL::object_cast<K::Segment_2>(&obj)) {
      //dual edge is finite
      K::Point_2 vor1 = seg->source();
      K::Point_2 vor2 = seg->target();
      K::FT max_bound = CGAL::squared_distance(v, vor1);
      max_bound = CGAL::max(max_bound, CGAL::squared_distance(v, vor2));
      bounds.emplace_back(max_bound, 1);
      
      if (CGAL::do_intersect(*seg, t.segment(it))) {
        K::FT min_bound = curr_edge_len / 4;
        bounds.emplace_back(min_bound, 0);
        if (min_bound <= s && s < max_bound) {
          a_s++;
        }
      } else {
        K::FT min_bound = CGAL::squared_distance(v, vor1);
        min_bound = CGAL::min(min_bound, CGAL::squared_distance(v, vor2));
        bounds.emplace_back(min_bound, 0);
        if (min_bound < s && s < max_bound) {
          a_s++;
        }
      }
      
    } else if (const K::Ray_2 * ray = CGAL::object_cast<K::Ray_2>(&obj)) {
      //dual edge is infinite
      K::FT min_bound;
      if (CGAL::do_intersect(*ray, t.segment(it))) {
        min_bound = curr_edge_len / 4;
        bounds.emplace_back(min_bound, 0);
        if (min_bound <= s) a_s++;
      } else {
        min_bound = CGAL::squared_distance(v, ray->source());
        bounds.emplace_back(min_bound, 0);
        if (min_bound < s) a_s++;
      }
      
    }
  }
  
  std::sort(bounds.begin(), bounds.end());
  int count = 0, max_count = 0;
  for (auto p : bounds) {
    if (p.second == 0) {
      count++;
      if (count > max_count) {
        max_count = count;
      }
    } else if (p.second == 1) {
      count--;
    }
  }
  
  std::cout << a_2 << ' ' << a_3 << ' ' << a_s << ' ' << max_count << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}