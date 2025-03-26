///2
#include <iostream>
#include <vector>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef std::vector<int> VI;
typedef std::vector<VI> VVI;
typedef std::vector<long> VL;
typedef std::vector<VL> VVL;

void solve(Program& lp, VVI& adj_list, VI& danger, VVL& resources, int root, int m, int prev, int& ind) {
  //if dangerous, assign an index
  int curr_ind, parent;
  if (danger[root] >= 0) {
    curr_ind = ind;
    ind += 1;
    parent = prev;
    prev = curr_ind;
  }

  //dfs order, so first solve for each child
  for (int child : adj_list[root]) {
    solve(lp, adj_list, danger, resources, child, m, prev, ind);
    if (danger[child] == -1) {
      //take resources if child is not dangerous
      for (int i = 0; i < m; i++) {
        resources[root][i] += resources[child][i];
      }
    }
  }
  
  //if the node is dangerous, set the relevant equations, and also set for parent
  if (danger[root] >= 0) {
    for (int i = 0; i < m; i++) {
      lp.set_a(curr_ind*m + i, curr_ind*(m+1) + 2*m + i, 1);  //resource
      lp.set_b(curr_ind*(m+1) + 2*m + i, resources[root][i]); //resource
      
      lp.set_a(curr_ind*m + i, curr_ind*(m+1) + 2*m + m, 1); //danger
    }
    lp.set_b(curr_ind*(m+1) + 2*m + m, danger[root]);
    
    if (parent == 0) {
      for (int i = 0; i < m; i++) {
        lp.set_a(curr_ind*m + i, i, -CGAL::Gmpq(1,2));
      }
    } else {
      for (int i = 0; i < m; i++) {
        lp.set_a(curr_ind*m + i, parent*(m+1) + 2*m + i, -CGAL::Gmpq(1,2));
      }
    }
    
    
  }
}

void testcase() {
  int n, m; std::cin >> n >> m;
  Program lp (CGAL::SMALLER, true, 0, false, 0); 
  
  VI danger;
  std::vector<std::vector<long>> resources(n, std::vector<long>(m, -1));
  for (int i = 0; i < n; i++) {
    int d; std::cin >> d;
    danger.push_back(d);
    for (int j = 0; j < m; j++) {
      int r_j; std::cin >> r_j;
      resources[i][j] = r_j;
    }
  }
  
  std::vector<std::vector<int>> adj_list(n);
  for (int i = 0; i < n-1; i++) {
    int u, v; std::cin >> u >> v;
    adj_list[v].push_back(u);
  }
  
  VI capacities, supplies, prices;
  for (int i = 0; i < m; i++) {
    int c, s, p; std::cin >> c >> s >> p;
    capacities.push_back(c);
    supplies.push_back(s);
    prices.push_back(p);
    
    lp.set_b(i, -c);
  }
  
  int ind = 1;
  solve(lp, adj_list, danger, resources, 0, m, 0, ind);
  for (int i = 0; i < m; i++) {
    lp.set_a(i, i + m, 1);
    lp.set_b(i + m, resources[0][i]);
    
    lp.set_a(i, i, -1);
  }
  
  for (int i = 0; i < m; i++) {
    lp.set_a(ind*m + i, i, -1); 
    lp.set_a(ind*m + i, ind*(m+1) + 2*m + i, 1);
    lp.set_b(ind*(m+1) + 2*m + i, supplies[i]);
    
    lp.set_c(ind*m + i, prices[i]);
  }
  
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if (s.is_infeasible()) {
    std::cout << "Impossible!" << std::endl;
  } else {
    std::cout << (long)CGAL::to_double(s.objective_value()) << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}