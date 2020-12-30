#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/bipartite.hpp>


typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef int Info;
typedef CGAL::Triangulation_vertex_base_with_info_2<Info,K> vertex_base;
typedef CGAL::Triangulation_face_base_2<K> face_base;
typedef CGAL::Triangulation_data_structure_2<vertex_base,face_base> data_structure;
typedef data_structure::Vertex_handle vertex_handle;
typedef CGAL::Delaunay_triangulation_2<K,data_structure> delaunay;
// typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;

typedef std::pair<K::Point_2,Info> point_info;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::out_edge_iterator out_edge_it;

bool bipartite(const graph& G, const delaunay& t, const int n, std::vector<int>& color, std::vector<int>& partition)
{
  std::queue<int> Q;
  Q.push(0);
  color[0] = 1;
  
  int idx = 0;
  int par = 0;
  partition[0] = par;
  
  while(!Q.empty()){
    int u = Q.front();
    Q.pop();
    
    out_edge_it ebeg, eend;
    for(boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg){
      int v = boost::target(*ebeg, G);
      if(color[u] == color[v]){
        return true;
      }
      else if(color[u] == -color[v]){
        continue;
      }
      else{
        color[v] = -color[u];
        Q.push(v);
        partition[v] = par;
      }
    }
    
    if(Q.empty()){
      while(idx < n){
        if(color[idx] != 0) idx++;
        else break;
      }
      if(idx < n){
        color[idx] = 1;
        Q.push(idx);
        
        par++;
        partition[idx] = par;
      }
    }
  }
  
  return false;
}

bool interfere(const std::vector<point_info>& stations, const std::vector<int>& color, const int n, const long r2)
{
  std::vector<point_info> black, white;
  black.reserve(n);
  white.reserve(n);
  for(int i=0; i<n; i++){
    if(color[i] == -1){
      black.push_back(stations[i]);
    }
    else if(color[i] == 1){
      white.push_back(stations[i]);
    }
  }
  
  delaunay tb, tw;
  tb.insert(black.begin(), black.end());
  tw.insert(white.begin(), white.end());
  
  for(auto e = tb.finite_edges_begin(); e != tb.finite_edges_end(); ++e){
    if(tb.segment(*e).squared_length() <= r2){
      return true;
    }
  }
  
  for(auto e = tw.finite_edges_begin(); e != tw.finite_edges_end(); ++e){
    if(tw.segment(*e).squared_length() <= r2){
      return true;
    }
  }
  
  return false;
}

void testcase()
{
  int n, m;
  long r;
  std::cin >> n >> m >> r;
  long r2 = r*r;
  
  // read input, and save together with indices (info)
  std::vector<point_info> stations;
  stations.reserve(n);
  for(int i=0; i<n; i++){
    int x, y;
    std::cin >> x >> y;
    stations.push_back({K::Point_2(x, y), i});
  }
  
  // delaunay triangulation
  delaunay t;
  t.insert(stations.begin(), stations.end());
  
  // make a graph and add edges from delaunay triangulation if the distance is <= r
  graph G(n);
  for(auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e){
    auto v1 = e->first->vertex((e->second+1) % 3);
    auto v2 = e->first->vertex((e->second+2) % 3);
    
    if(t.segment(*e).squared_length() <= r2){
      boost::add_edge(v1->info(), v2->info(), G);
    }
  }
  
  // check if the graph is bipartite && each network doesn't interfere within itself
  std::vector<int> color(n, 0);
  std::vector<int> partition(n, -1);
  bool not_possible = bipartite(G, t, n, color, partition);
  bool conflict = interfere(stations, color, n, r2);

  for(int i=0; i<m; i++){
    int x1, y1, x2, y2;
    std::cin >> x1 >> y1 >> x2 >> y2;
    K::Point_2 holmes(x1, y1), watson(x2, y2);
    
    /* check interference */
    if(not_possible || conflict){
      std::cout << "n";
      continue;
    }

    // check if they can communicate directly
    if(CGAL::squared_distance(holmes, watson) <= r2){
      std::cout << "y";
      continue;
    }
    
    // otherwise find the closest radio station and check if they can connect
    auto holmes_vertex = t.nearest_vertex(holmes);
    auto watson_vertex = t.nearest_vertex(watson);
    
    if((CGAL::squared_distance(holmes_vertex->point(), holmes) > r2) || (CGAL::squared_distance(watson_vertex->point(), watson) > r2)){
      std::cout << "n";
      continue;
    }
    
    // once they are connected to the closest station
    // check if they are in the same graph
    if(partition[holmes_vertex->info()] == partition[watson_vertex->info()]){
      std::cout << "y";
    }
    else{
      std::cout << "n";
    }
  }
  
  std::cout << std::endl;
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
