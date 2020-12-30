#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

void testcase(int n)
{
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for(int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  
  int m; std::cin >> m;
  std::vector<K::Point_2> query;
  query.reserve(m);
  for(int i=0; i<m; i++){
    int x, y;
    std::cin >> x >> y;
    query.push_back(K::Point_2(x, y));
  }
  
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  Triangulation::Face_handle f;
  for(auto q : query){
    auto p = t.nearest_vertex(q, f)->point();
    std::cout << std::fixed // to make it not use scientific expression
              << std::setprecision(0) // to make it not print after decimal point
              << CGAL::squared_distance(q, p) << "\n";
  }
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  while(true){
    int n; std::cin >> n;
    if(n == 0) break;
    testcase(n);
  }

  return 0;
}
