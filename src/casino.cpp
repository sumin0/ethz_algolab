#include <iostream>
#include <vector>
#include <map>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, int,
        boost::property<boost::edge_residual_capacity_t, int,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, int> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  edge_desc add_edge(int from, int to, int capacity, int cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
    
    return e;
  }
};


void testcase() {
  // Read input
  int n, m, l;
  std::cin >> n >> m >> l;
  
  // Create graph, edge adder class and propery maps
  graph G(n);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  // auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  auto w_map = boost::get(boost::edge_weight, G);
  
  // Add edges
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  adder.add_edge(v_source, 0, l, 0);
  adder.add_edge(n-1, v_sink, l, 0);
  
  const int max_q = 128;
  
  std::map<edge_desc, int> length;
  for(int i=0; i<m; ++i){
    int x, y, q; std::cin >> x >> y >> q;
    edge_desc e = adder.add_edge(x, y, 1, max_q*(y-x) - q);
    length.insert(std::make_pair(e, y-x));
  }
  
  for(int i=0; i<n-1; ++i){
    edge_desc e = adder.add_edge(i, i+1, l, max_q);
    length.insert(std::make_pair(e, 1));
  }
  
  // Run the algorithm
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  // int flow_cost = boost::find_flow_cost(G);
  
  int cost = 0;
  auto edge_iters = boost::edges(G);
  for (auto edge_it = edge_iters.first; edge_it != edge_iters.second; ++edge_it) {
    const edge_desc edge = *edge_it;
    const int flow_through_edge = c_map[edge] - rc_map[edge];
    
    if(flow_through_edge == 1){
      cost += length[edge] * max_q - w_map[edge];
    }
  }
  
  std::cout << cost << "\n";

}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();

  return 0;
}
