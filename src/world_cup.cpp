#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K> DT;


double floor_to_double(const CGAL::Quotient<ET>& x) {
    double a = floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a + 1 <= x) a += 1;
    return a;
}

void testcase()
{
  int n, m, c;
  std::cin >> n >> m >> c;
  
  // create an LP with Ax <= b and no lower/upper bounds
  Program lp (CGAL::SMALLER, false, 0, false, 0);
  // variable: transported amount a_(w,s)
  // (w,s) = (1,1), (1,2), (1,3), ...
  // constraint:
  // supply n
  // alcohol m
  // demand 2*m (equality)
  // lower bound:
  // a_(w,s) >= 0

  // warehouse pos, supply, alcohol content
  std::vector<K::Point_2> warehouse;
  warehouse.reserve(n);
  for (int i = 0; i < n; ++i) {
    int x, y, s_i, a_i;
    std::cin >> x >> y >> s_i >> a_i;
    warehouse.push_back(K::Point_2(x, y));
    
    // supply
    for(int j = 0; j < m; ++j){
      lp.set_a(i*m + j, i, 1);
    }
    lp.set_b(i, s_i);
    
    // upper limit
    for(int j = 0; j < m; ++j){
      lp.set_a(i*m + j, n+j, a_i);
    }
    
  }
  
  // stadium pos, demand, upper limit
  std::vector<K::Point_2> stadium;
  stadium.reserve(m);
  for (int j = 0; j < m; ++j){
    int x, y, d_j, u_j;
    std::cin >> x >> y >> d_j >> u_j;
    stadium.push_back(K::Point_2(x, y));
    
    // upper limit
    lp.set_b(n+j, u_j*100);
    
    // demand
    for(int i = 0; i < n; ++i){
      lp.set_a(i*m + j, n+m+j, 1);
      lp.set_a(i*m + j, n+m+m+j, -1);
    }
    lp.set_b(n+m+j, d_j);
    lp.set_b(n+m+m+j, -d_j);
    
  }

  // revenue
  std::vector<std::vector<int>> revenues(n, std::vector<int>(m, 0));
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < m; ++j){
      int r_ij;
      std::cin >> r_ij;
      revenues[i][j] = r_ij*100;
      // lp.set_c(i*m + j, -revenues[i][j]);
      
      // lower bound on a_(w,s)
      lp.set_l(i*m + j, true, 0);
    }
  }
  
  DT dt;
  dt.insert(warehouse.begin(), warehouse.end());
  dt.insert(stadium.begin(), stadium.end());
  
  // contour lines
  std::vector<K::Point_2> contour_p;
  std::vector<K::FT> contour_r2;
  contour_p.reserve(c);
  contour_r2.reserve(c);
  for (int k = 0; k < c; ++k){
    int x, y, r;
    std::cin >> x >> y >> r;
    
    K::Point_2 origin(x, y);
    auto pt = dt.nearest_vertex(origin);
    if (CGAL::squared_distance(pt->point(), origin) <= r*r){
      contour_p.push_back(origin);
      contour_r2.push_back(K::FT(r*r));
    }
  }
  
  c = contour_p.size();

  std::vector<std::vector<bool>> in_warehouse(c, std::vector<bool>(n, false));
  std::vector<std::vector<bool>> in_stadium(c, std::vector<bool>(m, false));
  for (int k = 0; k < c; ++k) {
    for (int i = 0; i < n; ++i) {
      if (CGAL::squared_distance(warehouse[i], contour_p[k]) <= contour_r2[k]){
          in_warehouse[k][i] = true;
      }
    }
    for (int j = 0; j < m; ++j){
      if (CGAL::squared_distance(stadium[j], contour_p[k]) <= contour_r2[k]){
          in_stadium[k][j] = true;
      }
    }
  }
  
  for (int i = 0; i < n; ++i){
    for (int j = 0; j < m; ++j){
      for(int k = 0; k < c; ++k){
        if (in_warehouse[k][i] != in_stadium[k][j]){
          revenues[i][j] -= 1;
        }
      }
      lp.set_c(i*m + j, -revenues[i][j]);
    }
  }
  
  // solve the program, using ET as the exact type
  Solution sol = CGAL::solve_linear_program(lp, ET());

  if(sol.is_infeasible()){
    std::cout << "RIOT!\n";
  }
  else{
    std::cout << floor_to_double(sol.objective_value() / ET(-100))
              << "\n";
  }
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  std::cout << std::fixed << std::setprecision(0);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
  
  return 0;
}
