#include <iostream>
#include <cmath>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT; // input type
typedef CGAL::Gmpz ET; // exact type for solver

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

void testcase()
{
  int xs, ys, n;
  std::cin >> xs >> ys >> n;
  
  Program lp = Program(CGAL::SMALLER, false, 0, false, 0);
  
  const int X = 0;
  const int Y = 1;
  const int T = 2;
  
  for(int i=0; i<n; i++){
    long a, b, c, v;
    std::cin >> a >> b >> c >> v;
    
    long k = a*xs + b*ys + c;
    long deno = sqrt(a*a + b*b);

    if(k > 0){
      lp.set_a(X, i, -a);
      lp.set_a(Y, i, -b);
      lp.set_a(T, i, v*deno);
      lp.set_b(i, c);
    }
    else{
      lp.set_a(X, i, a);
      lp.set_a(Y, i, b);
      lp.set_a(T, i, v*deno);
      lp.set_b(i, -c);
    }
  }
  
  lp.set_l(T, true, 0);
  lp.set_c(T, -1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  
//   if (s.is_infeasible()) {
//     std::cout << "infeasible\n";
//   }
//   else if (s.is_unbounded()) {
//     std::cout << "unbounded\n";
//   }
//   else
    std::cout << -s.objective_value_numerator() / s.objective_value_denominator() << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
}