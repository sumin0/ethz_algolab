#include <iostream>
#include <algorithm>
#include <limits>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef int Info;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Triangulation_face_base_with_info_2<Info, K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation;

typedef Triangulation::Finite_vertices_iterator Vertex_iterator;
typedef Triangulation::Finite_edges_iterator Edge_iterator;
typedef Triangulation::Finite_faces_iterator Face_iterator;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::property<boost::vertex_distance_t, long>,
                              boost::property<boost::edge_weight_t, long>
                              > weighted_graph;


void testcase(int n)
{
  std::vector<K::Point_2> pts;
  pts.reserve(n);
  for(int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    pts.push_back(K::Point_2(x, y));
  }
  
  Triangulation t;
  t.insert(pts.begin(), pts.end());
  
  // index faces
  int idx = 1;
  for(auto f = t.all_faces_begin(); f != t.all_faces_end(); ++f){
    if(t.is_infinite(f))
      f->info() = 0;
    else
      f->info() = idx++;
  }
  
  int v = idx;

  // construct a graph with weights = minimum distance 
  weighted_graph G(v);
  
  for(auto f = t.finite_faces_begin(); f != t.finite_faces_end(); ++f){
    for(int i=0; i<3; i++){
      auto v1 = f->vertex((i+1)%3)->point();
      auto v2 = f->vertex((i+2)%3)->point();
      // change data type
      long dx = long(v1.x()) - long(v2.x());
      long dy = long(v1.y()) - long(v2.y());
      long w = dx*dx + dy*dy;
      long inv_w = std::numeric_limits<long>::max() - w;
      
      boost::add_edge(f->info(), f->neighbor(i)->info(), inv_w, G);
    }
  }
  
  // Dijkstra
  std::vector<long> dist_map(v);
  dijkstra_shortest_paths(G, 0,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, G))).
    distance_combine([](long a, long b){return std::max(a,b);}));
  
  // narrowest part length
  for(int i=0; i<v; i++){
    dist_map[i] = std::numeric_limits<long>::max() - dist_map[i];
  }
  
  int m; std::cin >> m;
  for(int i=0; i<m; i++){
    int x, y; long d;
    std::cin >> x >> y >> d;
    
    K::Point_2 p(x, y);
    auto start = t.nearest_vertex(p);
    
    if(CGAL::squared_distance(p, start->point()) < d){
      std::cout << "n";
      continue;
    }
    
    // infinite
    auto f = t.locate(p);
    if(f->info() == 0){
      std::cout << "y";
      continue;
    }
    
    if(dist_map[f->info()] < 4*d){
      std::cout << "n";
    }
    else{
      std::cout << "y";
    }
    
  }
  std::cout << "\n";
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
