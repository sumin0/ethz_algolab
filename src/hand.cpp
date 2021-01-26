#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>

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


int max_family(const int k, const std::vector<int> & compGroup){
  if(k==1){
      return compGroup[3] + compGroup[2] + compGroup[1] + compGroup[0];
  }
  else if(k==2){
    return compGroup[3] + compGroup[2] + compGroup[1] + compGroup[0]/2;
  }
  else if(k==3){
    int match12 = std::min(compGroup[0], compGroup[1]);
    return (compGroup[3] + compGroup[2] + match12 +
      (compGroup[1]-match12)/2 + (compGroup[0]-match12)/3);
  }
  else{
    int match13 = std::min(compGroup[0], compGroup[2]);
    int cnt1 = compGroup[0] - match13;
    int cnt3 = compGroup[2] - match13;
    int tentWith2 = compGroup[1] + cnt3;
    return compGroup[3] + match13 + tentWith2/2 + ((tentWith2 & 1)*2 + cnt1)/4;
  }
  return 0;
}

void testcase()
{
  int n, k, f0; long s0;
  std::cin >> n >> k >> f0 >> s0;
  
  std::vector<std::pair<K::Point_2, int>> tents(n);
  for(int i=0; i<n; ++i){
    int x, y; std::cin >> x >> y;
    tents[i] = {{x, y}, i};
  }
  
  DT dt;
  dt.insert(tents.begin(), tents.end());

  std::vector<std::tuple<int, int, long>> edges;
  edges.reserve(3*n);
  for(auto e = dt.finite_edges_begin(); e != dt.finite_edges_end(); ++e){
    int v1 = e->first->vertex((e->second+1)%3)->info();
    int v2 = e->first->vertex((e->second+2)%3)->info();
    
    long len = dt.segment(e).squared_length();
    edges.emplace_back(v1, v2, len);
  }
  
  // faster
  boost::disjoint_sets_with_storage<> uf(n);
  
  // std::vector<int> component(n);
  // for(int i=0; i<n; ++i){
  //   component[i] = i;
  // }
  std::vector<int> tentPerComp(n, 1);
  std::vector<int> compGroup = {n, 0, 0, 0};

  std::sort(edges.begin(), edges.end(), [](const auto &e1, const auto &e2){
    return std::get<2>(e1) < std::get<2>(e2);
  });
  
  bool first_task = false;
  bool second_task = false;
  long max_s;
  int max_f = 1;
  int cur_max_f = n;
  for(auto [u, v, dist] : edges){
    int c1 = uf.find_set(u);
    int c2 = uf.find_set(v);
    if(c1 != c2) {
      cur_max_f = max_family(k, compGroup);
      
      if(!second_task && (dist >= s0)){
        max_f = cur_max_f;
        second_task = true;
      }
      
      if(cur_max_f >= f0){
        max_s = dist;
      }
      else{
        first_task = true;
      }
      
      if(first_task && second_task) break;
      
      compGroup[tentPerComp[c1]-1]--;
      compGroup[tentPerComp[c2]-1]--;
      int new_tent_c = std::min(tentPerComp[c1] + tentPerComp[c2], 4);
      compGroup[new_tent_c-1]++;
      tentPerComp[c1] = new_tent_c;
      tentPerComp[c2] = new_tent_c;
      
      // tentPerComp[c1] = tentPerComp[c1] + tentPerComp[c2];
      // tentPerComp[c2] = 0;
      // std::replace(component.begin(), component.end(), c2, c1);
      uf.link(c1, c2);
    }
  }
  
  std::cout << max_s << " " << max_f << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t; 
  while(t--)
    testcase();
    
  return 0;
}
