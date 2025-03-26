///1
#include <iostream>
#include <map>
#include <utility>
#include <tuple>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef K::Point_2 P;
typedef std::tuple<int, int, int> T;

void testcase() {
  int f, m, c, g; std::cin >> f >> m >> c >> g;
  int d, s; std::cin >> d >> s;
  
  std::map<P, T> map;
  std::vector<P> pts;
  for (int i = 0; i < s; i++) { //slytherin members
    int x_i, y_i, m_i, c_i, g_i;
    std::cin >> x_i >> y_i >> m_i >> c_i >> g_i;
    pts.emplace_back(x_i, y_i);
    map[P(x_i, y_i)] = T(m_i, c_i, g_i);
  }
  
  Triangulation t; //triangulation is only of the slytherin members
  t.insert(pts.begin(), pts.end()); //map stores their malfoy crab goyle leaks

  //Ax <= b, variable i denotes hours of felix felicis for person i
  Program lp (CGAL::SMALLER, true, 0, true, 24); //first pitfall, 24 hours is the upper bound
  
  std::map<P, int> slytherin_to_da;
  //map slytherin members to DA members; only remember the "best" DA member
  //slytherin members can be interrogated by only 1 DA member at a time
  for (int i = 0; i < d; i++) { //DA members
    int x_j, y_j, f_j;
    std::cin >> x_j >> y_j >> f_j;
    auto vh = t.nearest_vertex(P(x_j, y_j));
    
    if (slytherin_to_da.count(vh->point()) == 0) {
      slytherin_to_da[vh->point()] = f_j;
    } else {
      if (f_j < slytherin_to_da[vh->point()]) {
        slytherin_to_da[vh->point()] = f_j; //just remember the most efficient DA member
      }
    }
  }
  
  int ind = 0;
  for (auto p : slytherin_to_da) {
    P slytherin_member = p.first;
    int f_j = p.second; //the DA member doesn't matter beyond their f_j
    T triple = map[slytherin_member];
    int m_i = std::get<0>(triple);
    int c_i = std::get<1>(triple);
    int g_i = std::get<2>(triple);
    lp.set_a(ind, 0, -m_i); //sum of m_i contributions
    lp.set_a(ind, 1, -c_i); //sum of c_i contributions
    lp.set_a(ind, 2, -g_i); //sum of g_i contributions
    lp.set_a(ind, 3, f_j); //potion quantities <= f
    ind++;
  }

  lp.set_b(0, -m);
  lp.set_b(1, -c);
  lp.set_b(2, -g);
  lp.set_b(3, f);
  Solution sol = CGAL::solve_linear_program(lp, ET());
  
  if (sol.is_infeasible()) std::cout << "H\n";
  else std::cout << "L\n";
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}

