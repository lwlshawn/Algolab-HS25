///2
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <CGAL/enum.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 P;
typedef K::Line_2 L;

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;

bool is_contained(P v1, P v2, P v3, P p) {
  // points v1, v2, v3, are vertices of a triangle.
  // check if the point p is inside the triangle
  auto or1 = CGAL::orientation(v1, v2, p);
  auto or2 = CGAL::orientation(v2, v3, p);
  auto or3 = CGAL::orientation(v3, v1, p);
  
  return ((or1 >= 0 && or2 >= 0 && or3 >= 0) || (or1 <= 0 && or2 <= 0 && or3 <= 0));
}

void testcase() {
  int m, n; std::cin >> m >> n;
  
  std::vector<P> points;
  for (int i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    points.push_back(P(x,y));
  }
  
  // reading in the triangles
  VVI covered_by(n, VI());
  for (int i = 0; i < n; i++) {
    // for each pair of points, make a line
    int p1_x, p1_y; std::cin >> p1_x >> p1_y;
    int p2_x, p2_y; std::cin >> p2_x >> p2_y;    
    P p1(p1_x, p1_y);
    P p2(p2_x, p2_y);
    L l1(p1, p2);
    
    int p3_x, p3_y; std::cin >> p3_x >> p3_y;
    int p4_x, p4_y; std::cin >> p4_x >> p4_y;
    P p3(p3_x, p3_y);
    P p4(p4_x, p4_y);
    L l2(p3, p4);    
    
    int p5_x, p5_y; std::cin >> p5_x >> p5_y;
    int p6_x, p6_y; std::cin >> p6_x >> p6_y;
    P p5(p5_x, p5_y);
    P p6(p6_x, p6_y);
    L l3(p5, p6);
    
    // compute the three vertices of the triangle, and make the triangle
    auto r1 = CGAL::intersection(l1, l2);
    auto r2 = CGAL::intersection(l1, l3); 
    auto r3 = CGAL::intersection(l2, l3); 
    
    const P* v1 = boost::get<P>(&*r1);
    const P* v2 = boost::get<P>(&*r2);
    const P* v3 = boost::get<P>(&*r3);
  
    std::vector<bool> covers(m, false);
    for (int j = 0; j < m; j++) {
      if (is_contained(*v1, *v2, *v3, points[j])) covers[j] = true;
    }
    
    for (int j = 0; j < m - 1; j++) {
      if (covers[j] && covers[j+1]) covered_by[i].push_back(j);
    }
  }
  
  // sliding window time
  // base case
  int uncovered_count = m - 1;
  std::vector<int> counts(m-1, 0); //counts[i] = # times leg i covered
  for (int i : covered_by[0]) {
    counts[i]++;
    uncovered_count--;
  }
  
  if (uncovered_count == 0) {
    std::cout << 1 << std::endl;
    return;
  }
  
  // rest of triangles
  int best_cover = n+1;
  int l = 0;
  int r = 0;
  while (l < n && r < n) {
    if (uncovered_count != 0) {
      r++;
      if (r == n) {
        break;
      }
      
      for (int i : covered_by[r]) {
        if (counts[i] == 0) {
          uncovered_count--;
        }
        counts[i]++;
      }
      
    } else { //all legs covered.
        if (r - l + 1 < best_cover) {
          best_cover = r - l + 1;
        }
        
        //subtract counts
        for (int i : covered_by[l]) {
          counts[i]--;
          if (counts[i] == 0) {
            uncovered_count++;
          }
        }
        l++;
    }
  }
  std::cout << best_cover << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}