#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;


void testcase()
{
  int N, M, K, X, Y;
  std::cin >> N >> M >> K >> X >> Y;
  
  weighted_graph G(N*(K+1));
  
  for(int i=0; i<M; ++i){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    
    // (K+1) layer
    // between the layers: edges along the river
    // within one layer: all edges
    if(d){
      for(int j=0; j<K; ++j){
        boost::add_edge(a + j*N, b + (j+1)*N, c, G); // connection between j-th and (j-1)-th layer
        boost::add_edge(b + j*N, a + (j+1)*N, c, G);
      }
    }
    
    for(int j=0; j<K+1; ++j){
      boost::add_edge(a + j*N, b + j*N, c, G); // connection within j-th layer
      boost::add_edge(b + j*N, a + j*N, c, G);
    }
  }
  
  std::vector<int> dist_map(N*(K+1));
  dijkstra_shortest_paths(G, X,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, G))));
    
  std::cout << dist_map[Y+N*K] << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}