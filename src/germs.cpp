#include <iostream>
#include <algorithm>
#include <map>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;


double round_up(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while(a < x) a += 1;
  while(a-1 > x) a -= 1;
  return a;
}

void testcase(int n)
{
  int left, bottom, right, top;
  std::cin >> left >> bottom >> right >> top;
  
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for(int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  std::map<K::Point_2, double> dist;
  // to the boundary
  for(Vertex_iterator v=t.finite_vertices_begin(); v!=t.finite_vertices_end(); ++v ){
    K::FT d = std::min({v->point().x() - left,
                       right - v->point().x(),
                       v->point().y() - bottom,
                       top - v->point().y()});
    dist[v->point()] = CGAL::to_double(d*d);
  }
  
  // between points
  for(Edge_iterator e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
    Triangulation::Vertex_handle v1 = e->first->vertex((e->second+1) % 3);
    Triangulation::Vertex_handle v2 = e->first->vertex((e->second+2) % 3);
    
    double d = CGAL::to_double(CGAL::squared_distance(v1->point(), v2->point())) / 4.;
    dist[v1->point()] = std::min(dist[v1->point()], d);
    dist[v2->point()] = std::min(dist[v2->point()], d);
  }
  
  // sort - using vector
  std::vector<double> vec_d;
  vec_d.reserve(n);
  for(auto map_d : dist){
    vec_d.push_back(map_d.second);
  }
  std::sort(vec_d.begin(), vec_d.end());
  
  auto toTime = [](const double &d){
    return round_up(sqrt(sqrt(d) - 0.5));
  };
  
  std::cout << toTime(vec_d[0]) << " " << toTime(vec_d[n/2]) << " " << toTime(vec_d[n-1]) << "\n";

}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  std::cout << std::fixed // to make it not use scientific expression
            << std::setprecision(0); // to make it not print after decimal point
  
  while(true){
    int n; std::cin >> n;
    if(n == 0) break;
    testcase(n);
  }

  return 0;
}
