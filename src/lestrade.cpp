#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// typedef CGAL::Delaunay_triangulation_2<K> DT;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> DT;

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct Info{
  int u, v, w;
};

void testcase()
{
  int feeHolmes, where, who, how;
  std::cin >> feeHolmes >> where >> who >> how;
  
  int a, g;
  std::cin >> a >> g;
  
  std::vector<std::pair<K::Point_2, int>> gangs(g);
  std::vector<Info> g_info(g);
  for(int i=0; i<g; i++){
    int x, y, u, v, w;
    std::cin >> x >> y >> u >> v >> w;
    
    gangs[i] = {{x, y}, i};
    g_info[i] = {u, v, w};
  }
  
  DT dt;
  dt.insert(gangs.begin(), gangs.end());
  
  std::vector<int> gang2cost(g, 1025);
  for(int i=0; i<a; i++){
    int x, y, z;
    std::cin >> x >> y >> z;
    int nearest_gang = dt.nearest_vertex(K::Point_2(x, y))->info();
    gang2cost[nearest_gang] = std::min(z, gang2cost[nearest_gang]);
  }
  
  Program lp (CGAL::LARGER, true, 0, true, 24);
  
  int agent_var = 0;
  for(int i=0; i<g; i++){
    if(gang2cost[i] > 1024) continue;
    
    lp.set_a(agent_var, 0, g_info[i].u);
    lp.set_a(agent_var, 1, g_info[i].v);
    lp.set_a(agent_var, 2, g_info[i].w);    
    
    lp.set_c(agent_var, gang2cost[i]);
    
    agent_var++;
  }
  
  lp.set_b(0, where);
  lp.set_b(1, who);
  lp.set_b(2, how);
  
  Solution sol = CGAL::solve_linear_program(lp, ET());
  
  if(sol.is_infeasible() || sol.objective_value() > feeHolmes)
    std::cout << "H\n";
  else
    std::cout << "L\n";
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--){
    testcase();
  }
  
  return 0;
}