///2
#include <iostream>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef K::Point_2 P;
typedef Triangulation::Vertex_handle Vertex_handle;

void testcase(int n) {
  std::vector<P> pts;
  for (int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    pts.push_back(P(x, y));
  }
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  std::cout << std::fixed << std::setprecision(0);
  int m; std::cin >> m;
  for (int i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    P pt = P(x,y);
    Vertex_handle closest_pt = t.nearest_vertex(pt);
    std::cout << CGAL::squared_distance(closest_pt->point(), pt) << std::endl;
  }
}


int main()
{
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}
