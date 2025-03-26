///4
#include <iostream>
#include <vector>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase() {
  int n, m, c; std::cin >> n >> m >> c;
  
  // create an LP with Ax <= b, lower bound 0 and no upper bounds
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  int ind = 0;

  std::vector<int> alcohol;
  std::vector<K::Point_2> warehouses;
  for (int i = 0; i < n; ++i) {
    int x, y, s, a; std::cin >> x >> y >> s >> a;
    warehouses.emplace_back(x,y);
    alcohol.push_back(a);
    
    for (int j = 0; j < m; j++) {
      lp.set_a(i*m + j, ind, 1);
    }
    lp.set_b(ind, s);
    ind++;
  }
  
  std::vector<K::Point_2> stadiums;
  for (int j = 0; j < m; j++) {
    int x, y, d, u; std::cin >> x >> y >> d >> u;
    stadiums.emplace_back(x,y);
    
    for (int i = 0; i < n; i++) {
      lp.set_a(i*m + j, ind, -1);
    }
    lp.set_b(ind, -d);
    ind++;
    
    for (int i = 0; i < n; i++) {
      lp.set_a(i*m + j, ind, 1);
    }
    lp.set_b(ind, d);
    ind++;
    
    for (int i = 0; i < n; i++) {
      lp.set_a(i*m + j, ind, alcohol[i]);
    }
    lp.set_b(ind, 100*u);
    ind++;
  }

  std::vector<std::vector<int>> rev(n, std::vector<int>(m, -1));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      int r_ij; std::cin >> r_ij;
      rev[i][j] = 100 * r_ij;
    }
  }
  
  Triangulation t;
  t.insert(warehouses.begin(), warehouses.end());
  t.insert(stadiums.begin(), stadiums.end());
  
  for (int i = 0; i < c; i++) {
    int x, y; long r; std::cin >> x >> y >> r;
    K::Point_2 center(x, y);
    K::Circle_2 contour(center, r*r);
    auto vh = t.nearest_vertex(center);
    if (CGAL::squared_distance(vh->point(), center) <= r*r) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
          K::Point_2 warehouse = warehouses[i];
          K::Point_2 stadium = stadiums[j];
          if (contour.bounded_side(warehouse) != contour.bounded_side(stadium)) {
            rev[i][j]--;
          }
        }
      }
    }
  }
  
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      lp.set_c(i*m + j, -rev[i][j]);
    }
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_infeasible()) std::cout << "no" << std::endl;
  else std::cout << ((long)std::floor(-CGAL::to_double(s.objective_value()) / 100)) << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}