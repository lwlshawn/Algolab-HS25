// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int d) {
  
  Program lp (CGAL::SMALLER, false, 0, false, 0); 
  std::vector<std::vector<int>> inequalities;
  // variables x_0,...,x_{d-1} are variables 0 up to d-1
  // variable d represents the radius of the circle r we want to maximise
  // so objective is to minimise -r
  std::vector<long> norms;

  for (int i = 0; i < n; i++) {
    std::vector<int> inequality; //stores all the coefficient a_iT and b_i
    
    double norm = 0;
    for (int j = 0; j < d; j++) {
      int a_i; std::cin >> a_i;
      inequality.push_back(a_i);
      norm += a_i * a_i;
    }
    
    long rounded_norm = std::round(std::sqrt(norm));
    int b_i; std::cin >> b_i;
    inequality.push_back(b_i);
    norms.push_back(rounded_norm);
    
    //x = (x_0,...,x_{d-1}) are the coordinates of the circle. Should satisfy planes
    for (int j = 0; j < d; j++) { //j here is the variables for the circle
      //lp.set_a(variable, equation, coeff)
      lp.set_a(j, i, inequality[j]);
    }
    //lp.set_b(equation, value)
    lp.set_b(i, inequality[d]); //the dth element
    inequalities.push_back(inequality);
  }
  
  for (int i = 0; i < n; i++) {
    //in addition, the center should be sufficiently far from the planes.
    // r || a || <= b - a.x - abs omitted due to halfplane inequalities
    // r || a || + a.x <= b
    lp.set_a(d, n + i, norms[i]); //we just use d as index for radius we want to maximise
    for (int j = 0; j < d; j++) {
      lp.set_a(j, n + i, inequalities[i][j]);
    }
    lp.set_b(n + i, inequalities[i][d]);
  }
  
  lp.set_c(d, -1);
  Solution s = CGAL::solve_linear_program(lp, ET());
  
  if (s.is_infeasible()) {
    std::cout << "none" << std::endl;
  } else if (s.is_unbounded()) {
    std::cout << "inf" << std::endl;
  } else {
    std::cout << int(std::floor(-CGAL::to_double(s.objective_value()))) << std::endl;
  }

}


int main() {
  std::ios_base::sync_with_stdio(false);
  int n; std::cin >> n;
  while (n != 0) {
    int d; std::cin >> d;
    testcase(n, d);
    std::cin >> n;
  }
  
}

