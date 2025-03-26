///1

#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Segment_2 S;
typedef K::Ray_2 R;

double floor_to_double(const K::FT& x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}

void testcase(int n) {
  long x, y, a, b; std::cin >> x >> y >> a >> b;
  P source(x, y);
  R ray(source, P(a,b));
  
  std::vector<S> segments;
  for (int i = 0; i < n; i++) {
    long r, s, t, u; std::cin >> r >> s >> t >> u;
    segments.push_back(S(P(r,s), P(t,u)));
  }
  std::random_shuffle(segments.begin(), segments.end());
  
  
  bool have_intersect = false;
  K::FT smallest_squared_distance;
  S cut_ray;
  for (S s : segments) {
    if (have_intersect) {
      if (CGAL::do_intersect(s, cut_ray)) {
        auto res = CGAL::intersection(s, cut_ray);
        if (const P* p = boost::get<P>(&*res)) {
          cut_ray = S(source, *p);
          smallest_squared_distance = cut_ray.squared_length();
          
        } else if (const S* s = boost::get<S>(&*res)) {
          S s1 = S(source, s->source());
          S s2 = S(source, s->target());
          if (s1.squared_length() < s2.squared_length()) {
            cut_ray = s1;
          } else {
            cut_ray = s2;
          }
          
        } else {
          throw std::runtime_error("weird intersect");
        }
      }
    } else {
      // this block only executes once; against the first segment
      // we find that intersects the ray
      if (CGAL::do_intersect(s, ray)) {
        have_intersect = true;
        
        auto res = CGAL::intersection(s, ray);
        
        if (const P* p = boost::get<P>(&*res)) {
          cut_ray = S(source, *p);
          smallest_squared_distance = cut_ray.squared_length();
          
        } else if (const S* s = boost::get<S>(&*res)) {
          S s1 = S(source, s->source());
          S s2 = S(source, s->target());
          if (s1.squared_length() < s2.squared_length()) {
            cut_ray = s1;
          } else {
            cut_ray = s2;
          }
          
        } else {
          throw std::runtime_error("weird intersect");
        }
      }
    }
  }
  
  if (!have_intersect) {
    std::cout << "no" << std::endl;
  } else {
    K::FT x = cut_ray.target().x();
    K::FT y = cut_ray.target().y();
    std::cout << std::fixed << std::setprecision(0);
    std::cout << floor_to_double(x) << ' ' << floor_to_double(y) << std::endl;
  }
  
}


int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    testcase(n);
    std::cin >> n;
  }
}