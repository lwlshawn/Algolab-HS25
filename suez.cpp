///3
#include <iostream>
#include <vector>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;


void testcase() {
  int n, m, h, w; std::cin >> n >> m >> h >> w;
  std::vector<K::Point_2> new_nails(n), old_nails(m);

  for (int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    new_nails[i] = K::Point_2(x, y);
  }
  
  for (int i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    old_nails[i] = K::Point_2(x, y);
  }
  
  // create an LP with Ax <= b, lower bound 1 and no upper bounds
  Program lp (CGAL::SMALLER, true, 1, false, 0); 
  
  int eq_ind = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i+1; j < n; j++) {
      K::Point_2 p1 = new_nails[i];
      K::Point_2 p2 = new_nails[j];
      
      lp.set_a(i, eq_ind, 1);
      lp.set_a(j, eq_ind, 1);
      
      CGAL::Gmpq bound1 = CGAL::abs(CGAL::Gmpq(2*p1.x() - 2*p2.x(), w));
      CGAL::Gmpq bound2 = CGAL::abs(CGAL::Gmpq(2*p1.y() - 2*p2.y(), h));

      lp.set_b(eq_ind, CGAL::max(bound1, bound2));
      eq_ind++;
    }
  }
  
  for (int i = 0; i < n; i++) {
    K::Point_2 p1 = new_nails[i];
    CGAL::Gmpq tight_bound = -1;
    for (int j = 0; j < m; j++) {
      K::Point_2 p2 = old_nails[j];
      CGAL::Gmpq bound1 = CGAL::abs(CGAL::Gmpq(2*p1.x() - 2*p2.x(), w)) - 1;
      CGAL::Gmpq bound2 = CGAL::abs(CGAL::Gmpq(2*p1.y() - 2*p2.y(), h)) - 1;
      CGAL::Gmpq curr_bound = CGAL::max(bound1, bound2);
      
      if (tight_bound == -1) tight_bound = curr_bound;
      else tight_bound = CGAL::min(tight_bound, curr_bound);
    }
    
    if (tight_bound != -1) {
      lp.set_a(i, eq_ind, 1);
      lp.set_b(eq_ind, tight_bound);
      eq_ind++;
    }

  }
  
  
  for (int i = 0; i < n; i++) {
    lp.set_c(i, -2*(h + w));
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  // for (auto it = s.variable_values_begin(); it != s.variable_values_end(); it++) {
  //   std::cerr << (*it) << ' ';
  // }
  // std::cerr << std::endl;
  
  
  std::cout << (long)(std::floor(-CGAL::to_double(s.objective_value()))) << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
