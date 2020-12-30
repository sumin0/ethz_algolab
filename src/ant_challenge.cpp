#include <iostream>
#include <vector>
#include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int>> Graph;
typedef property_map<Graph, edge_weight_t>::type WeightMap;
typedef graph_traits<Graph>::vertex_descriptor   Vertex;
typedef graph_traits<Graph>::edge_descriptor     Edge;
typedef graph_traits<Graph>::edge_iterator       EdgeIt;

void testcase() {
  int numTree, numEdge, numSpecies, a, b;
  std::cin >> numTree >> numEdge >> numSpecies >> a >> b;
  
  std::vector<Graph> graphs(numSpecies);
  std::vector<WeightMap> weights(numSpecies);
  
  for(int i = 0; i < numSpecies; i++) {
    graphs[i] = Graph(numTree);
    weights[i] = get(edge_weight, graphs[i]);
  }
  
  for(int i = 0; i < numEdge; i++) {
    int t1, t2; std::cin >> t1 >> t2;
    
    for (int j = 0; j < numSpecies; j++) {
      Edge e; bool success;
      tie(e, success) = add_edge(t1, t2, graphs[j]);
      
      int w; std::cin >> w;
      weights[j][e] = w;
    }
  }
  
  std::vector<int> h(numSpecies);
  for (int i = 0; i < numSpecies; i++) {
    std::cin >> h[i];
  }
  
  // combine graphs
  Graph G(numTree);
  WeightMap wm = get(edge_weight, G);
  
  for (int i = 0; i < numSpecies; i++) {
    std::vector<Edge> mst;
    kruskal_minimum_spanning_tree(graphs[i], back_inserter(mst));
    
    for (auto edge : mst) {
      Edge e; bool success;
      
      tie(e, success) = add_edge(source(edge, graphs[i]), target(edge, graphs[i]), G);
      wm[e] = get(edge_weight, graphs[i], edge);
    }
  }
  
  // find the shortest path
  std::vector<int> dist_map(numEdge);
  
  dijkstra_shortest_paths(G, a, distance_map(make_iterator_property_map(dist_map.begin(), get(vertex_index, G))));
  
  std::cout << dist_map[b] << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();

  return 0;
}