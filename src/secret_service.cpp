#include <iostream>
#include <vector>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor Vertex;

void testcase()
{
  int N, M, A, S, C, D;
  std::cin >> N >> M >> A >> S >> C >> D;
  
  weighted_graph G(N);
  // weight_map wm = boost::get(boost::edge_weight, G);
  
  for(int m=0; m<M; ++m){
    char w; int x, y, z;
    std::cin >> w >> x >> y >> z;
    
    boost::add_edge(x, y, z, G);
    if(w == 'L'){
      boost::add_edge(y, x, z, G);
    }
  }
  
  // compute for each agent
  std::vector<std::vector<int>> dist_map(A, std::vector<int>(N));
  for(int a=0; a<A; ++a){
    int i; std::cin >> i;
    dijkstra_shortest_paths(G, i, boost::distance_map(boost::make_iterator_property_map(
    dist_map[a].begin(), boost::get(boost::vertex_index, G))));
  }
  
  // pairwise distance matrix T from agents to shelters
  std::vector<std::vector<int>> T(A, std::vector<int>(S));
  for(int s=0; s<S; ++s){
    int j; std::cin >> j;
    for(int a=0; a<A; ++a){
      T[a][s] = dist_map[a][j];
    }
  }
  
  // binary search
  int low = 0, high = std::numeric_limits<int>::max();
  while(low < high){
    int mid = low + (high-low)/2;
    weighted_graph GG(A+C*S);
    for(int a=0; a<A; ++a){
      for(int s=0; s<S; ++s){
        if(T[a][s] == std::numeric_limits<int>::max()) continue; // unreachable
        
        for(int c=0; c<C; ++c){
          if(T[a][s] + (c+1)*D <= mid){
            boost::add_edge(a, A + c*S + s, GG);
          }
        }
      }
    }
    
    // compute maximum cardinality matching
    std::vector<Vertex> matching(A + C*S);
    edmonds_maximum_cardinality_matching(GG, boost::make_iterator_property_map(
    matching.begin(), boost::get(boost::vertex_index, GG)));
    
    const Vertex null_vertex = boost::graph_traits<weighted_graph>::null_vertex();
    
    int matching_size = 0;
    for(int a=0; a<A; ++a){
      matching_size += (matching[a] != null_vertex);
    }
    
    if(matching_size == A){
      high = mid;
    }
    else{
      low = mid+1;
    }
  }
  
  std::cout << low << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
}