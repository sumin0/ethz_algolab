#include <iostream>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase(int n, int d){
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  const int R = d;
  for(int i=0; i<n; i++){
    double ai_norm = 0;
    for(int j=0; j<d; j++){
      int ai; std::cin >> ai;
      lp.set_a(j, i, ai);
      ai_norm += ai*ai;
    }
    int b; std::cin >> b;
    lp.set_b(i, b);
    lp.set_a(R, i, sqrt(ai_norm));
  }
  
  lp.set_l(R, true, 0);
  lp.set_c(R, -1);
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()){
    std::cout << "none\n";
  }
  else if(s.is_unbounded()){
    std::cout << "inf\n";
  }
  else{
    int sol = std::floor(-CGAL::to_double(s.objective_value()));
    std::cout << sol << std::endl;
  }
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int n, d; std::cin >> n;
  while(n != 0){
    std::cin >> d;
    testcase(n, d);
    std::cin >> n;
  }

  return 0;
}