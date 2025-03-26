#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <tuple>
#include <deque>

#include <CGAL/enum.h>
#include <CGAL/Gmpq.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Ray_2 R;

void testcase() {
  // std::cerr << "testcase begins" << std::endl;
  int n; std::cin >> n;
  std::deque<std::tuple<long, R, P, int>> motorcycles;
  for (int i = 0; i < n; i++) {
    long y_0, x_1, y_1; std::cin >> y_0 >> x_1 >> y_1;
    P p1(0, y_0);
    P p2(x_1, y_1);
    motorcycles.emplace_back(y_0, R(p1, p2), p1, i);
  }
  
  //sorted in descending starting order along the starting line
  std::sort(motorcycles.begin(), motorcycles.end(), [](std::tuple<long,R,P,int> a, std::tuple<long,R,P,int> b)
                                                      {return std::get<0>(a) > std::get<0>(b);});
  
  std::deque<std::tuple<long, R, P, int>> sunset;
  while (motorcycles.size() > 0) {
    auto curr_motorcycle = motorcycles.front();
    if (sunset.size() == 0) {
      sunset.push_back(curr_motorcycle);
      motorcycles.pop_front();
    } else {
      auto old_motorcycle = sunset.back();
      // std::cerr << "comparing: " << std::get<3>(old_motorcycle) << ' ' << std::get<3>(curr_motorcycle) << std::endl;
      
      R r_old = std::get<1>(old_motorcycle);
      R r_new = std::get<1>(curr_motorcycle);
      
      if (!CGAL::do_intersect(r_old, r_new)) {
        sunset.push_back(curr_motorcycle);
        motorcycles.pop_front();
      } else {
        P old_start = std::get<2>(old_motorcycle);
        P new_start = std::get<2>(curr_motorcycle);
        auto o = CGAL::intersection(r_old, r_new);
        P intersection_point = boost::get<P>(*o);
        if (CGAL::has_larger_distance_to_point(intersection_point, new_start, old_start)) {
          // std::cerr << std::get<3>(old_motorcycle) << " wins " << std::endl;
          motorcycles.pop_front();
        } else {
          // std::cerr << std::get<3>(curr_motorcycle) << " wins " << std::endl;
          sunset.pop_back();
        }
        
      }
    }
  }
  
  std::vector<int> sunset_indexes;
  for (auto t : sunset) {
    sunset_indexes.push_back(std::get<3>(t));
  }
  
  std::sort(sunset_indexes.begin(), sunset_indexes.end());
  for (int ind : sunset_indexes) {
    std::cout << ind << ' ';
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
