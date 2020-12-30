#include <iostream>
#include <vector>
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


void testcase() {
  // Read input
  int N, M, S;
  std::cin >> N >> M >> S;
  
  std::vector<int> limit(S);
  for(int i=0; i<S; ++i){
    std::cin >> limit[i];
  }
  
  std::vector<int> land2state(M);
  for(int i=0; i<M; ++i){
    std::cin >> land2state[i];
  }
  
  std::vector<std::vector<int>> bid(N, std::vector<int>(M));
  for(int i=0; i<N; i++){
    for(int j=0; j<M; j++){
      std::cin >> bid[i][j];
    }
  }

  // Create graph, edge adder class and propery maps
  graph G(S+M+N);
  edge_adder adder(G);  
  auto c_map = boost::get(boost::edge_capacity, G);
  // auto r_map = boost::get(boost::edge_reverse, G);
  auto rc_map = boost::get(boost::edge_residual_capacity, G);
  
  // Add edges
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  for(int i=0; i<S; ++i){
    adder.add_edge(v_source, i, limit[i], 0); // source to state
  }
  
  for(int i=S; i<S+M; ++i){
    adder.add_edge(land2state[i - S] - 1, i, 1, 0); // state to land
    for(int j=S+M; j<S+M+N; ++j){
      int cost = 100 - bid[j - (S+M)][i - S]; // land to buyer
      adder.add_edge(i, j, 1, cost);
    }
  }
  
  for(int i=S+M; i<S+M+N; ++i){
    adder.add_edge(i, v_sink, 1, 0); // buyer to sink
  }
  
  // Run the algorithm
  // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
  int flow_cost = boost::find_flow_cost(G);
  
  // Iterate over all edges leaving the source to sum up the flow values.
  int s_flow = 0;
  out_edge_it e, eend;
  for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
      s_flow += c_map[*e] - rc_map[*e];     
  }
  
  int profit = s_flow * 100 - flow_cost;
  std::cout << s_flow << " " << profit << std::endl;

}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();

  return 0;
}
