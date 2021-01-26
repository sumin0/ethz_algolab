#include <iostream>
#include <vector>
#include <limits>

// BGL includes
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

struct Guide_info
{
  int x, y, d, e;
};

bool isPossible(const std::vector<Guide_info> & guide, const int num_city, const int budget, const int start, const int end, const int flow){
  graph G(num_city);
  edge_adder adder(G);
  
  const auto v_source = boost::add_vertex(G);
  const auto v_target = boost::add_vertex(G);
  
  adder.add_edge(v_source, start, flow, 0);
  adder.add_edge(end, v_target, flow, 0);
  
  for(int i=0; i<guide.size(); ++i){
    adder.add_edge(guide[i].x, guide[i].y, guide[i].e, guide[i].d);
  }
  
  boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
  int cost = boost::find_flow_cost(G);
  
  return (cost <= budget);
}

void testcase()
{
  int c, g, b, k, a;
  std::cin >> c >> g >> b >> k >> a;
  
  graph G(c);
  edge_adder adder(G);
  
  const auto v_source = boost::add_vertex(G);
  const auto v_target = boost::add_vertex(G);
  
  std::vector<Guide_info> guide(g);
  for(int i=0; i<g; ++i){
    int x, y, d, e;
    std::cin >> x >> y >> d >> e;
    guide[i] = {x, y, d, e};

    adder.add_edge(x, y, e, d);
  }
  
  adder.add_edge(v_source, k, std::numeric_limits<int>::max(), 0);
  adder.add_edge(a, v_target, std::numeric_limits<int>::max(), 0);
  
  int max_flow = boost::push_relabel_max_flow(G, v_source, v_target);
  // std::cout << max_flow << " ";
  
  // binary search
  int start = 0;
  int end = max_flow;
  while(start != end){
    int num_bag = (start + end + 1)/2;
    
    if(isPossible(guide, c, b, k, a, num_bag)){
      start = num_bag;
    }
    else{
      end = num_bag -1;
    }
  }
  
  std::cout << start << std::endl;

}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}