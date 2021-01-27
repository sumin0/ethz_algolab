#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
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

struct State{
  int idx;
  int node, t;
  
  bool operator<(const State& other) const{
    if(node == other.node){
      if(t == other.t)
        return idx < other.idx;
      return t < other.t;
    }
    return node < other.node;
  }
};

struct StatePair{
  State s1, s2;
  
  bool operator<(const StatePair& other) const{
    return s1 < other.s1;
  }
};

void testcase()
{
  int N, S;
  std::cin >> N >> S;
  
  graph G(2*N+S);
  edge_adder adder(G);
  
  const auto v_source = boost::add_vertex(G);
  const auto v_target = boost::add_vertex(G);
  
  std::vector<State> pairs;
  pairs.reserve(2*N + S);
  
  int num_cars = 0;
  int idx = 0;
  for(int i=0; i<S; ++i){
    int l; std::cin >> l;
    adder.add_edge(v_source, idx, l, 0);
    pairs.push_back({idx++, i, 0});
    num_cars += l;
  }
  
  int max_time = 0;
  std::map<StatePair, int> profit;
  for(int i=0; i<N; ++i){
    int s, t, d, a, p;
    std::cin >> s >> t >> d >> a >> p;
    State s1 = {idx++, s-1, d};
    State s2 = {idx++, t-1, a};
    
    pairs.push_back(s1);
    pairs.push_back(s2);
    
    max_time = std::max(max_time, a);
    
    profit[{s1, s2}] = p;
  }
  
  std::sort(pairs.begin(), pairs.end());
  
  for(int i=0; i < 2*N+S-1; ++i){
    // connect within the same node (different time stamps)
    if(pairs[i+1].node == pairs[i].node){
      int time_diff = pairs[i+1].t - pairs[i].t;
      if(time_diff == 0){
        adder.add_edge(pairs[i].idx, pairs[i+1].idx, num_cars, 0);
        adder.add_edge(pairs[i+1].idx, pairs[i].idx, num_cars, 0);
      }
      else{
        adder.add_edge(pairs[i].idx, pairs[i+1].idx, num_cars, 100*time_diff);
      }
    }
    // connect the last state of each node to target
    else{
      adder.add_edge(pairs[i].idx, v_target, num_cars, 100*(max_time-pairs[i].t));
    }
  }
  adder.add_edge(pairs.back().idx, v_target, num_cars, 100*(max_time-pairs.back().t));

  // connect the requests
  for(auto it = profit.begin(); it != profit.end(); ++it){
    int time_diff = it->first.s2.t - it->first.s1.t;
    adder.add_edge(it->first.s1.idx, it->first.s2.idx, 1, 100*time_diff - it->second);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int min_cost = boost::find_flow_cost(G);
  
  std::cout << 100*max_time*num_cars - min_cost << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}