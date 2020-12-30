#include <iostream>
#include <queue>
#include <algorithm>
#include <numeric>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/prim_minimum_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace boost;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::property<boost::vertex_distance_t, int>,
                              boost::property<boost::edge_weight_t, int>
                              > weighted_graph;
                              
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type WeightMap;
typedef boost::property_map<weighted_graph, boost::vertex_index_t>::type IndexMap;


void testcase()
{
  int v, e; std::cin >> v >> e;
  weighted_graph G(v);
  
  for(int i=0; i<e; i++){
    int m1, m2, w; std::cin >> m1 >> m2 >> w;
    add_edge(m1, m2, w, G);
  }
  
  // prim
  std::vector<vertex_desc> mst_pred(v);
  std::vector<vertex_desc> mst_dist(v);
  WeightMap weight_map = get(edge_weight, G);
  IndexMap idx_map = get(vertex_index, G);
  
  prim_minimum_spanning_tree(G, *vertices(G).first, &mst_pred[0], &mst_dist[0],
    weight_map, idx_map, default_dijkstra_visitor());

  int weight_sum = std::accumulate(mst_dist.begin(), mst_dist.end(), 0);
  
  // Dijkstra
  std::vector<int> dist_map(v);
  dijkstra_shortest_paths(G, 0,
    distance_map(make_iterator_property_map(
    dist_map.begin(), get(vertex_index, G))));
  
  int max_dist = *std::max_element(dist_map.begin(), dist_map.end());
  
  std::cout << weight_sum << " " << max_dist << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for(int i=0; i<t; i++){
    testcase();
  }

  return 0;
}
