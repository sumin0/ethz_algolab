#include <iostream>
#include <cmath>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>


typedef CGAL::Gmpz IT;
typedef CGAL::Gmpz ET;

typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


struct Cell{
  int x, y, z;
};

bool isSeparable(const std::vector<Cell>& healthy, const std::vector<Cell>& tumor, const int degree)
{
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  
  int var_index = 1;
  for(int i=0; i<=degree; i++){
    for(int j=0; j<=degree - i; j++){
      for(int k=0; k<=degree - i - j; k++){
        
        for(size_t h=0; h<healthy.size(); h++){
          IT coeff = pow(healthy[h].x, i)*pow(healthy[h].y, j)*pow(healthy[h].z, k);
          lp.set_a(var_index, h, coeff);
        }
        
        for(size_t t=0; t<tumor.size(); t++){
          IT coeff = -pow(tumor[t].x, i)*pow(tumor[t].y, j)*pow(tumor[t].z, k);
          lp.set_a(var_index, t+healthy.size(), coeff);
        }
        var_index++;
        
      }
    }
  }
  
  for(size_t h=0; h<healthy.size(); h++){
    lp.set_a(0, h, 1);
    lp.set_b(h, 0);
  }
  
  for(size_t t=0; t<tumor.size(); t++){
    lp.set_a(0, t+healthy.size(), 1);
    lp.set_b(t+healthy.size(), 0);
  }
  
  lp.set_l(0, true, 0); // has to be positive
  lp.set_c(0, -1); // maximize
  
  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);
  Solution s = CGAL::solve_linear_program(lp, ET(), options);
  
  // std::cout << s.objective_value() << std::endl;

  if(s.is_infeasible() || (s.objective_value() == 0 && !s.is_unbounded())){
    return false;
  }
  else{
    return true;
  }
}

void testcase(){
  int h, t; std::cin >> h >> t;
  
  std::vector<Cell> healthy;
  healthy.reserve(h);
  for(int i=0; i<h; i++){
    int x, y, z; std::cin >> x >> y >> z;
    healthy.push_back({x, y, z});
  }
  
  std::vector<Cell> tumor;
  tumor.reserve(t);
  for(int i=0; i<t; i++){
    int x, y, z; std::cin >> x >> y >> z;
    tumor.push_back({x, y, z});
  }
  
  int min_d = 31;
  for(int i=0; i<=30; i++){
    if(isSeparable(healthy, tumor, i)){
      min_d = i;
      break;
    }
  }
  
  if(min_d > 30)
    std::cout << "Impossible!\n";
  else
    std::cout << min_d << "\n";
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();

  return 0;
}