#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <vector>
#include <iostream>
#include <algorithm>

// typedefs
typedef  CGAL::Exact_predicates_inexact_constructions_kernel K;

int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    long x, y, a, b;
    std::cin >> x >> y >> a >> b;
    K::Ray_2 ray(K::Point_2(x, y), K::Point_2(a, b));

    std::vector<K::Segment_2> Obs;
    for (int i = 0; i < n; i++) {
      long r, s, t, u;
      std::cin >> r >> s >> t >> u;
      K::Point_2 p1(r, s);
      K::Point_2 p2(t, u);
      Obs.push_back(K::Segment_2(p1, p2));
    }
    std::random_shuffle(Obs.begin(), Obs.end());
    
    bool intersect = false;
    for (K::Segment_2 seg : Obs) {
      if (CGAL::do_intersect(seg, ray)) {
        std::cout << "yes" << std::endl;
        intersect = true;
        break;
      }
    }
    
    if (!intersect) {
      std::cout << "no" << std::endl;
    }
    std::cin >> n;
  }
}
