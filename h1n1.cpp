///1
#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <utility>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Face_handle Fh;

void testcase(int n) {
  std::vector<K::Point_2> pts;
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    pts.emplace_back(x, y);
  }
  
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  std::map<Fh, K::FT> dist;
  typedef std::pair<K::FT, Fh> P;
  std::priority_queue<P, std::vector<P>> pq;
  const long INF = 1000000000;
  
  auto vh = t.infinite_vertex();
  auto circ = t.incident_faces(vh);
  do {
    dist[circ] = INF;
    pq.push(P(INF, circ));
  } while (++circ != t.incident_faces(vh));
  
  while (!pq.empty()) {
    auto p = pq.top();
    pq.pop();
    
    K::FT curr_dist = p.first;
    Fh curr_face = p.second;
    if (curr_dist < dist[curr_face]) continue;

    for (int i = 0; i <= 2; i++) {
      Fh nbr = curr_face->neighbor(i);
      if (t.is_infinite(nbr)) continue;
      
      auto vh1 = curr_face->vertex((i + 1) % 3);
      auto vh2 = curr_face->vertex((i + 2) % 3);
      K::FT edge_len = CGAL::squared_distance(vh1->point(), vh2->point());

      if (t.is_infinite(curr_face)) {
        if (dist.count(nbr) == 0 || edge_len > dist[nbr]) {
          dist[nbr] = edge_len;
          pq.push(P(dist[nbr], nbr));
        }
      } else {
        if (dist.count(nbr) == 0 || CGAL::min(edge_len, dist[curr_face]) > dist[nbr]) {
          dist[nbr] = CGAL::min(edge_len, dist[curr_face]);
          pq.push(P(dist[nbr], nbr));
        }
      }
    }
  }
  
  int m; std::cin >> m;
  for (int i = 0; i < m; i++) {
    int x, y; long d; std::cin >> x >> y >> d;
    
    K::Point_2 query(x,y);
    auto vh = t.nearest_vertex(query);
    
    if (CGAL::squared_distance(vh->point(), query) < d) {
      std::cout << "n";
    } else { //distance at least d
      Fh containing_face = t.locate(query);
      if (t.is_infinite(containing_face)) {
        std::cout << "y";
      } else {
        if (dist[containing_face] >= 4 * d) {
          std::cout << "y";
        } else {
          std::cout << "n";
        }
      }
    }
  }
  std::cout << std::endl;

}


int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }

}
