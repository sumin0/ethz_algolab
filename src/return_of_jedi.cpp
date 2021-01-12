#include <iostream>
#include <vector>
#include <limits>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int>> Graph;
typedef boost::property_map<Graph, boost::edge_weight_t>::type WeightMap;
typedef boost::graph_traits<Graph>::vertex_descriptor   Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor     Edge;
typedef boost::graph_traits<Graph>::edge_iterator       EdgeIt;


void testcase()
{
  int n, idx; std::cin >> n >> idx;
  
  Graph G(n);
  Graph MST(n);
  
  std::vector<std::vector<int>> cost(n, std::vector<int>(n));
  for(int j=0; j<n-1; ++j){
    for(int k=0; k<n-j-1; ++k){
      int c; std::cin >> c;
      cost[j][j+k+1] = c;
      cost[j+k+1][j] = c;
      
      boost::add_edge(j, j+k+1, c, G);
    }
  }
  
  std::vector<Edge> mst;
  boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
  
  int MSTcost = 0;
  std::vector<std::vector<bool>> isInMST(n, std::vector<bool>(n, false));
  for(auto e : mst){
    int u = boost::source(e, G);
    int v = boost::target(e, G);
    isInMST[u][v] = true;
    isInMST[v][u] = true;
    
    MSTcost += cost[u][v];
    boost::add_edge(u, v, cost[u][v], MST);
  }
  
  // maximum cost
  std::vector<std::vector<int>> maxEdgeCost(n);
  for(int i=0; i<n; i++){
    std::vector<int> dist_map(n);
    dijkstra_shortest_paths(MST, i,
      boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))).
      distance_combine([](int a, int b){return std::max(a,b);}));
      
      maxEdgeCost[i] = dist_map;
  }
  
  // add a new edge that connects (u, v)
  // remove the max. cost edge between (u, v)
  int minDiff = std::numeric_limits<int>::max();
  for(int j=0; j<n-1; ++j){
    for(int k=0; k<n-j-1; ++k){
      if(isInMST[j][j+k+1]) continue;
      
      int diff = cost[j][j+k+1] - maxEdgeCost[j][j+k+1];
      minDiff = std::min(minDiff, diff);
    }
  }
  
  std::cout << MSTcost + minDiff << "\n";
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}