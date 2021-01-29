#include <iostream>
#include <vector>
#include <algorithm>
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
  void add_edge(int from, int to, int capacity, int cost) {
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
  }
};

int MAX_C = 50;

void testcase() {
  // Read input
  int b, s, p;
  std::cin >> b >> s >> p;
  
  // Create graph, edge adder class and propery maps
  graph G(b+s);
  edge_adder adder(G);  
  
  // Add edges
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  
  for(int i=0; i<b; ++i){
    adder.add_edge(v_source, i, 1, 0);
  }
  
  // need to make the flow constant so it optimizes the cost only
  // if b >= s, gives a boat a choice to go to the sink directly
  // if it goes directly, it means the pair is not chosen
  // this way we can keep the max flow constant
  if(b >= s){
    for(int i=0; i<b; ++i){
      adder.add_edge(i, v_sink, 1, MAX_C);
    }
  }
  // vice versa
  else{
    for(int i=0; i<s; ++i){
      adder.add_edge(v_source, b+i, 1, MAX_C);
    }
  }
  
  for(int i=0; i<p; ++i){
    int bi, si, ci; std::cin >> bi >> si >> ci;
    int cost = MAX_C - ci;
    adder.add_edge(bi, b+si, 1, cost);
  }
  
  
  for(int i=0; i<s; ++i){
    adder.add_edge(b+i, v_sink, 1, 0);
  }
  
  // Run the algorithm
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int flow_cost = boost::find_flow_cost(G);
  
  int max_spectacle = std::max(b, s)*MAX_C - flow_cost;
  std::cout << max_spectacle << "\n";

}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
}
