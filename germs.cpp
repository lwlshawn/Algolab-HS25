#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt KS;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

double ceil_to_double(const KS::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a-1 >= x) a -=1;
  return a;
}

KS::FT compute_time(KS::FT Q) {
  //Q = 4t^4 + 4t^2 + 1. Ignoring negative cases
  KS::FT temp = CGAL::sqrt(Q) - 1;
  return CGAL::sqrt((temp / 2));
}

void testcase(int n) {
  int lft, bot, rgt, top; std::cin >> lft >> bot >> rgt >> top;
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    pts.emplace_back(x, y);
  }
  // construct triangulation
  Triangulation t;
  t.insert(pts.begin(), pts.end()); 
  
  std::vector<K::FT> order_deaths;
  //for each bacteria
  for (auto it = t.finite_vertices_begin(); it != t.finite_vertices_end(); it++) {
    K::Point_2 b_i = it->point();
    //for each boundary - find closest. Scaled by 4 for comparison with neighbours.
    K::FT b_dist = 4 * CGAL::squared_distance(b_i, K::Point_2(lft, b_i.y()));
    b_dist = CGAL::min(b_dist, 4 * CGAL::squared_distance(b_i, K::Point_2(rgt, b_i.y())));
    b_dist = CGAL::min(b_dist, 4 * CGAL::squared_distance(b_i, K::Point_2(b_i.x(), top)));
    b_dist = CGAL::min(b_dist, 4 * CGAL::squared_distance(b_i, K::Point_2(b_i.x(), bot)));
    
    auto vcirc = t.incident_vertices(it);
    if (vcirc != nullptr) {
      do {
        if (!t.is_infinite(vcirc)) { //THIS WAS THE BUG
          b_dist = CGAL::min(b_dist, CGAL::squared_distance(b_i, vcirc->point()));
        }
      } while (++vcirc != t.incident_vertices(it));
    }
    
    order_deaths.push_back(b_dist);
  }
  
  std::sort(order_deaths.begin(), order_deaths.end()); //non-descending order
  KS::FT first_death = order_deaths[0];
  KS::FT last_death = order_deaths[n-1];
  KS::FT mid_death = order_deaths[n/2];
  
  double first_death_time = ceil_to_double(compute_time(first_death));
  double mid_death_time = ceil_to_double(compute_time(mid_death));
  double last_death_time = ceil_to_double(compute_time(last_death));
  
  std::cout << std::fixed << std::setprecision(0);
  std::cout << first_death_time << ' ' << mid_death_time << ' ' << last_death_time << std::endl;
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}


