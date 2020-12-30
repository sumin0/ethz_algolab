#include <iostream>
// #include <vector>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef int IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


void testcase(int n, int m){
  Program lp (CGAL::SMALLER, true, 0, false, 0);
  
  for(int i=0; i<n; i++){
    int min, max; std::cin >> min >> max;
    lp.set_b(2*i, max);
    lp.set_b(2*i+1, -min);
  }
  
  for(int j=0; j<m; j++){
    int p; std::cin >> p;
    lp.set_c(j, p);
    for(int i=0; i<n; i++){
      int c; std::cin >> c;
      lp.set_a(j, 2*i, c);
      lp.set_a(j, 2*i+1, -c);
    }
  }
  
  Solution s = CGAL::solve_linear_program(lp, ET());
  if(s.is_infeasible()){
    std::cout << "No such diet.\n";
  }
  else{
    int sol = std::floor(CGAL::to_double(s.objective_value()));
    std::cout << sol << std::endl;
  }
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int n, m; std::cin >> n >> m;
  while(n != 0){
    testcase(n, m);
    std::cin >> n >> m;
  }

  return 0;
}