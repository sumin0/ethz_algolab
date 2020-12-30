#include <iostream>
#include <vector>
#include <utility>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/properties.hpp>
#include <boost/graph/biconnected_components.hpp>

using namespace boost;

namespace boost
{
  struct edge_component_t
  {
    enum
    { num = 555 };
    typedef edge_property_tag kind;
  }
  edge_component;
}

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_component_t, int>> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::edge_iterator edge_it;
typedef property_map<Graph, edge_component_t>::type ComponentMap;
typedef property_map<Graph, vertex_index_t>::type IndexMap;

void testcase()
{
  int n, m; std::cin >> n >> m;
  
  Graph G(n);
  for(int i=0; i<m; i++){
    int e1, e2; std::cin >> e1 >> e2;
    add_edge(e1, e2, G);
  }
  
  ComponentMap component = get(edge_component, G);
  int num = biconnected_components(G, component);
  
  edge_it e_beg, e_end;
  std::vector<int> comp(num, 0);
  for(tie(e_beg, e_end) = edges(G); e_beg!=e_end; ++e_beg){
    comp[component[*e_beg]]++;
  }
  
  IndexMap index = get(vertex_index, G);
  
  std::vector<std::pair<int, int>> edge;
  for(tie(e_beg, e_end) = edges(G); e_beg!=e_end; ++e_beg){
    if(comp[component[*e_beg]] == 1){
      int i = index[source(*e_beg, G)];
      int j = index[target(*e_beg, G)];
      if(i > j){
        std::swap(i, j);
      }
      edge.push_back(std::make_pair(i, j));
    }
    // std::cout << component[*e_beg] << " ";
  }
  std::sort(edge.begin(), edge.end());
  
  std::cout << edge.size() << "\n";
  for(auto x : edge)
    std::cout << x.first << " " << x.second << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();

  return 0;
}