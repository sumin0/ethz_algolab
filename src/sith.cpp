#include <iostream>
#include <vector>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

typedef int Info;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<Info,K> vertex_base;
typedef CGAL::Triangulation_face_base_2<K> face_base;
typedef CGAL::Triangulation_data_structure_2<vertex_base,face_base> data_structure;
typedef data_structure::Vertex_handle vertex_handle;
typedef CGAL::Delaunay_triangulation_2<K,data_structure> DT;

typedef std::pair<K::Point_2, Info> idx_pt;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

int max_connected_component(const std::vector<idx_pt> &planets, const int k, const long long r2)
{
  DT dt;
  dt.insert(planets.begin() + k, planets.end());
  
  int N = planets.size() - k;
  graph G(N);
  
  for(auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e){
    auto source = e->first->vertex((e->second+1) % 3);
    auto target = e->first->vertex((e->second+2) % 3);
    
    if(dt.segment(e).squared_length() <= r2){
      boost::add_edge(source->info() - k, target->info() - k, G);
    }
  }
  
  std::vector<int> component(N);
  int num_comp = boost::connected_components(G, &component[0]);
  
  // count
  std::vector<int> cnt_comp(num_comp, 0);
  for(int i=0; i<N; ++i){
    cnt_comp[component[i]] += 1;
  }
  
  return *(std::max_element(cnt_comp.begin(), cnt_comp.end()));
  
}

void testcase()
{
  int n, r; std::cin >> n >> r;
  
  std::vector<idx_pt> planets(n);
  for(int i=0; i<n; ++i){
    int x, y; std::cin >> x >> y;
    planets[i] = std::make_pair(K::Point_2(x, y), i);
  }
  
  // caution
  long long r2 = r;
  r2 *= r2;
  
  // binary search [left, right) range
  int left = 1;
  int right = n/2+1;
  while(left+1 < right){
    int mid = (left + right)/2;
    
    int k = max_connected_component(planets, mid, r2);
    if(mid <= k){
      left = mid;
    }
    else{
      right = mid;
    }
  }
  
  std::cout << left << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}