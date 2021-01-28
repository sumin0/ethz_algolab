#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;


class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

struct Edge{
  int u, v, capacity;
  int weight;
};

void testcase()
{
  int n, m, s, f;
  std::cin >> n >> m >> s >> f;
  
  weighted_graph SP(n);
  
  std::vector<Edge> roads(m);
  for(int i=0; i<m; ++i){
    int a, b, c, d;
    std::cin >> a >> b >> c >> d;
    boost::add_edge(a, b, d, SP); // length
    roads[i] = {a, b, c, d}; // width, length
  }
  
  // run Dijkstra to get shortest paths
  std::vector<int> dist_map(n);
  dijkstra_shortest_paths(SP, s,
    boost::distance_map(boost::make_iterator_property_map(
    dist_map.begin(), boost::get(boost::vertex_index, SP))));
  
  graph MF(n);
  edge_adder adder(MF);
  
  // add edge only if it is one of the shortest paths
  // check this by comparing the edge weight (road length) with the distance map value
  for(auto e : roads){
    if(e.weight == (dist_map[e.v]-dist_map[e.u])){
      adder.add_edge(e.u, e.v, e.capacity);
    }
    else if(e.weight == (dist_map[e.u]-dist_map[e.v])){
      adder.add_edge(e.v, e.u, e.capacity);
    }
  }
  
  long flow = boost::push_relabel_max_flow(MF, s, f);
  std::cout << flow << std::endl;
  
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