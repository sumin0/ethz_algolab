#include <iostream>
#include <iomanip>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

// struct Point{
//   int x;
//   int y;
//   Point(int a, int b) : x(a), y(b) {}
// };

void testcase()
{
  // create an LP with Ax <= b and no lower/upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  // sewer canal: x + ay + b = 0
  // fresh water canal: -ax + y + c = 0
  const int a = 0;
  const int b = 1;
  const int c = 2;
  const int l = 3;
  
  long a_coeff = 0;
  long b_coeff = 0;
  long const_coeff = 0;
  
  int n, m; long s;
  std::cin >> n >> m >> s;

  // noble
  for (int i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    
    // sewer canal on left
    lp.set_a(a, i, y);
    lp.set_a(b, i, 1);
    lp.set_b(i, -x);
    
    // sum of length -x -ay -b
    a_coeff += -y;
    b_coeff += -1;
    const_coeff += x;
    
    // longest
    lp.set_a(a, n+m+ 2*i, x);
    lp.set_a(c, n+m+ 2*i, -1);
    lp.set_a(l, n+m+ 2*i, -1);
    lp.set_b(n+m+ 2*i, y);
    
    lp.set_a(a, n+m+ 2*i+1, -x);
    lp.set_a(c, n+m+ 2*i+1, 1);
    lp.set_a(l, n+m+ 2*i+1, -1);
    lp.set_b(n+m+ 2*i+1, -y);
  }

  // commoner
  for (int i = 0; i < m; ++i) {
    int x, y; std::cin >> x >> y;
    
    // sewer canal on right
    lp.set_a(a, n+i, -y);
    lp.set_a(b, n+i, -1);
    lp.set_b(n+i, x);
    
    // sum of length x +ay +b
    a_coeff += y;
    b_coeff += 1;
    const_coeff += -x;
    
    // the longest
    lp.set_a(a, 3*n+m+ 2*i, x);
    lp.set_a(c, 3*n+m+ 2*i, -1);
    lp.set_a(l, 3*n+m+ 2*i, -1);
    lp.set_b(3*n+m+ 2*i, y);
    
    lp.set_a(a, 3*n+m+ 2*i+1, -x);
    lp.set_a(c, 3*n+m+ 2*i+1, 1);
    lp.set_a(l, 3*n+m+ 2*i+1, -1);
    lp.set_b(3*n+m+ 2*i+1, -y);
  }
  
  // minimize the longest fresh water pipe
  lp.set_l(l, true, 0);
  lp.set_c(l, 1);

  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());

  // Yuck
  if(sol.is_infeasible()){
    std::cout << "Yuck!\n";
    return;
  }
  
  // Bankrupt
  if(s == 0){
    std::cout << "Bankrupt!\n";
    return;
  }
  if(s == -1){
    std::cout << std::ceil(CGAL::to_double(sol.objective_value())) << "\n";
    return;
  }
  
  lp.set_a(a, 3*n+3*m+ 1, a_coeff);
  lp.set_a(b, 3*n+3*m+ 1, b_coeff);
  lp.set_b(3*n+3*m+ 1, s+const_coeff);
    
  Solution sol_Tywin = CGAL::solve_linear_program(lp, ET());
    
  if(sol_Tywin.is_infeasible()){
    std::cout << "Bankrupt!\n";
  }
  else{
    std::cout << std::ceil(CGAL::to_double(sol_Tywin.objective_value())) << "\n";
  }
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  std::cout << std::fixed << std::setprecision(0);
  
  int t; std::cin >> t;
  while(t--){
    testcase();
  }
  
  return 0;
}
