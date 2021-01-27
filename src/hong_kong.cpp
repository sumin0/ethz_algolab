#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>

// we need EXACT constructions
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

struct int_FT{
  int idx;
  K::FT length;
  
  bool operator<(const int_FT& other) const{
    return length < other.length;
  }
};

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K>    Vb;
typedef CGAL::Triangulation_face_base_with_info_2<int_FT, K>   Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  DT;
typedef std::pair<K::Point_2, int> Point;


K::FT MAX_VALUE = K::FT(std::numeric_limits<long>::max()) * K::FT(std::numeric_limits<long>::max());

void get_lb(std::vector<K::FT> & path_width,  const std::vector<std::vector<int_FT>> & edges){
  
  std::priority_queue<int_FT> q;
  q.push({0, MAX_VALUE});

  while(!q.empty()){
    int u = q.top().idx;
    K::FT max_width = q.top().length;
    q.pop();

    if(path_width[u] != -1){
      continue;
    }
    path_width[u] = max_width;

    for(auto & neighbor : edges[u]){
      int v = neighbor.idx;
      K::FT width = neighbor.length;
      if(path_width[v] == -1){
        q.push({v, std::min(path_width[u], width)});
      }
    }
  }
}

void testcase()
{
  int n, m; long r;
  std::cin >> n >> m >> r;
  
  // trees
  std::vector<Point> tree(n);
  for(int i=0; i<n; ++i){
    long x, y; std::cin >> x >> y;
    tree[i] = std::make_pair(K::Point_2(x, y), i);
  }
  
  DT dt;
  dt.insert(tree.begin(), tree.end());
  
  // save clearance
  int num_face = 1;
  for(auto f = dt.all_faces_begin(); f != dt.all_faces_end(); ++f){
    if(dt.is_infinite(f)){
      f->info().idx = 0;
    }
    else{
      K::Point_2 center = CGAL::circumcenter(f->vertex(0)->point(), f->vertex(1)->point(), f->vertex(2)->point());
      f->info().length = CGAL::squared_distance(center, f->vertex(0)->point());
      f->info().idx = num_face++;
    }
  }
  
  // connect faces
  std::vector<std::vector<int_FT>> edges(num_face);
  for(auto f = dt.finite_faces_begin(); f != dt.finite_faces_end(); ++f){
    int current_face = f->info().idx;
    
    for(int i=0; i<3; i++){
      int next_face = f->neighbor(i % 3)->info().idx;
      
      auto v1 = f->vertex((i+1)%3)->point();
      auto v2 = f->vertex((i+2)%3)->point();
      
      K::FT width = CGAL::squared_distance(v1, v2);
      
      edges[current_face].push_back({next_face, width});
      edges[next_face].push_back({current_face, width});
    }
    
    // for extra safe spots
    edges[current_face].push_back({0, f->info().length});
    edges[0].push_back({current_face, f->info().length});
  }
  
  // get lower bound of width along paths (similar to dijkstra)
  std::vector<K::FT> path_width(num_face, -1);
  get_lb(path_width, edges);
  
  // balloons
  for(int i=0; i<m; ++i){
    long x, y, s; std::cin >> x >> y >> s;
    K::Point_2 pt(x, y);
    
    K::FT thre = K::FT(r + s) * K::FT(r + s);
    
    // check if we can inflate
    if(CGAL::squared_distance(pt, dt.nearest_vertex(pt)->point()) < thre){
      std::cout << "n";
      continue;
    }
    
    auto f = dt.locate(pt);
    if(path_width[f->info().idx] >= 4*thre){
      std::cout << "y";
    }
    else
      std::cout << "n";
    
  }
  
  std::cout << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}