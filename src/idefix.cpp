#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> DT;
typedef DT::Finite_vertices_iterator Vertex_iterator;

// similar structure with hand.cpp
void testcase()
{
  int n, m, k; long s;
  std::cin >> n >> m >> s >> k;
  
  std::vector<std::pair<K::Point_2, int>> trees(n);
  for(int i=0; i<n; ++i){
    int x, y; std::cin >> x >> y;
    trees[i] = {{x, y}, i};
  }
  
  DT dt;
  dt.insert(trees.begin(), trees.end());

  // store the nearest tree idx and the distance from it
  std::vector<std::vector<long>> bones(n);
  for(int i=0; i<m; ++i){
    int x, y; std::cin >> x >> y;
    K::Point_2 pt(x, y);
    auto nearest_v = dt.nearest_vertex(pt);
    long dist = 4 * CGAL::squared_distance(nearest_v->point(), pt);
    
    bones[nearest_v->info()].push_back(dist);
  }
  
  for(int i=0; i<n; ++i){
    std::sort(bones[i].begin(), bones[i].end());
  }

  std::vector<std::tuple<int, int, long>> edges;
  edges.reserve(3*n);
  for(auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e){
    int v1 = e->first->vertex((e->second+1)%3)->info();
    int v2 = e->first->vertex((e->second+2)%3)->info();
    
    long len = dt.segment(e).squared_length();
    edges.emplace_back(v1, v2, len);
  }
  
  std::sort(edges.begin(), edges.end(), [](const auto &e1, const auto &e2){
    return std::get<2>(e1) < std::get<2>(e2);
  });
  
  boost::disjoint_sets_with_storage<> uf(n);
  
  // initialize
  // no components are connected
  int a = 0;
  long q = std::numeric_limits<long>::max();
  for(auto bone : bones){
    int a_can = std::upper_bound(bone.begin(), bone.end(), s) - bone.begin();
    a = std::max(a, a_can);
    if(bone.size() >= k){
      q = std::min(q, bone[k-1]);
    }
  }
  
  // start connecting
  int cnt = 1;
  for(auto [u, v, dist] : edges){
    int c1 = uf.find_set(u);
    int c2 = uf.find_set(v);
    if(c1 != c2) {
      uf.link(c1, c2);
      int c = uf.find_set(c1);
      
      // connect the component
      std::vector<long> merged_bone;
      std::merge(bones[c1].begin(), bones[c1].end(),
        bones[c2].begin(), bones[c2].end(), std::back_inserter(merged_bone));
      bones[c] = merged_bone;
      
      // get minimum radius
      // big enough to include k bones && all the edges
      if(bones[c].size() >= k){
        q = std::min(q, std::max(bones[c][k-1], dist));
      }
      // get maximum number of bones
      // among connected component c
      if(dist <= s){
        auto it = std::upper_bound(bones[c].begin(), bones[c].end(), s);
        a = std::max(a, int(std::distance(bones[c].begin(), it)));
      }
      
      if(++cnt == n) break;
    }
  }
  
  std::cout << a << " " << q << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; 
  while(t--)
    testcase();
    
  return 0;
}
