#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

#include <vector>
#include <iostream>
#include <iomanip>

// typedefs
typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

long ceil_to_long(const K::FT& x) {
  double a = std::ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -=1;
  return long(a);
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    std::vector<K::Point_2> P;
    for (int i = 0; i < n; i++) {
      long x, y; std::cin >> x >> y;
      P.push_back(K::Point_2(x, y));
    }
    
    Min_circle mc(P.begin(), P.end(), true);
    Traits::Circle c = mc.circle();
    K::FT r = CGAL::sqrt(c.squared_radius());
    std::cout << ceil_to_long(r) << "\n";
    
    std::cin >> n;
  }
}


  // const int n = 100;
  // std::vector<K::Point_2> P;
  
  // for (int i = 0; i < n; ++i)
  //   P.push_back(K::Point_2((i % 2 == 0 ? i : -i), 0));
  // // (0,0), (-1,0), (2,0), (-3,0), ...
  
  // Min_circle mc(P.begin(), P.end(), true);
  // Traits::Circle c = mc.circle();
  // std::cout << c.center() << " " << c.squared_radius() << "\n";