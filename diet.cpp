// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase(int n, int m) {
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  std::vector<std::pair<int, int>> nutrients;
  for (int i = 0; i < n; i++) {
    int min_i, max_i; std::cin >> min_i >> max_i;
    nutrients.emplace_back(min_i, max_i);
  }
  
  std::vector<std::vector<int>> foods;
  for (int i = 0; i < m; i++) {
    std::vector<int> food; //foods[i] stores a vector [p_j, C_j1, C_j2,...,C_jn]
    for (int j = 0; j < n + 1; j++) {
      int x; std::cin >> x;
      food.push_back(x);
    }
    foods.push_back(food);
  }
  
  for (int i = 0; i < n; i++) { //for each nutrient, define two constraints
    for (int j = 0; j < m; j++) { //each food product participates in each constraint
      lp.set_a(j, 2*i, foods[j][i+1]); // Sum(x_j * C_{j,i}) <= max_i
      lp.set_a(j, 2*i+1, -foods[j][i+1]); //Sum(-1 * x_j * C_{j,i} <= -min_i)
    }
    lp.set_b(2*i, nutrients[i].second);
    lp.set_b(2*i+1, -nutrients[i].first);
  }
  
  for (int j = 0; j < m; j++) {
    lp.set_c(j, foods[j][0]); //minimise Sum(x_j * p_j)
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  assert(s.solves_linear_program(lp));
  if (s.is_infeasible()) {
    std::cout << "No such diet." << std::endl;
  } else if (s.is_optimal()) {
    std::cout << std::floor(CGAL::to_double(s.objective_value())) << std::endl;
  }
  
}

int main() {
  int n, m; std::cin >> n >> m;
  while (n != 0 && m != 0) {
    testcase(n, m);
    std::cin >> n >> m;
  }
}


// int main()
// {
//   // create an LP with Ax <= b, lower bound 0 and no upper bounds
  
//   // set the coefficients of A and b
//   const int X = 0; 
//   const int Y = 1;
//   lp.set_a(X, 0,  1); lp.set_a(Y, 0, 1); lp.set_b(0, 7);  //  x + y  <= 7
//   lp.set_a(X, 1, -1); lp.set_a(Y, 1, 2); lp.set_b(1, 4);  // -x + 2y <= 4
//   // set upper bound
//   lp.set_u(Y, true, 4);                                   //       y <= 4
//   // objective function
//   lp.set_c(Y, -32);                                       // -32y
//   lp.set_c0(64);                                          // +64

//   // solve the program, using ET as the exact type
//   Solution s = CGAL::solve_linear_program(lp, ET());
//   assert(s.solves_linear_program(lp));
  
//   // output solution
//   std::cout << s; 
// }
