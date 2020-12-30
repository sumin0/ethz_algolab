#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    
    // if(boost::edge(from, to, G).second){
    //   const auto e = boost::edge(from, to, G).first;
      
    //   c_map[e] += capacity;
    // }
    // else{
      const auto e = boost::add_edge(from, to, G).first;
      const auto rev_e = boost::add_edge(to, from, G).first;
    
      c_map[e] = capacity;
      c_map[rev_e] = 0; // reverse edge has no capacity!
      r_map[e] = rev_e;
      r_map[rev_e] = e;
    // }
  }
};

void testcase()
{
  int n, m, k, c;
  std::cin >> n >> m >> k >> c;
  
  graph G(2*n*m);
  edge_adder adder(G);
  
  // Make a grid
  for(int i=0; i<n; i++){
    for(int j=0; j<m-1; j++){
      adder.add_edge(2*(n* j    + i)+1, 2*(n*(j+1) + i), 1);
      adder.add_edge(2*(n*(j+1) + i)+1, 2*(n* j    + i), 1);
    }
  }
  
  for(int i=0; i<n-1; i++){
    for(int j=0; j<m; j++){
      adder.add_edge(2*(n*j +  i)+1, 2*(n*j + (i+1)), 1);
      adder.add_edge(2*(n*j + (i+1))+1, 2*(n*j  + i), 1);
    }
  }
  
  for(int i=0; i<n; i++){
    for(int j=0; j<m; j++){
      adder.add_edge(2*(n*j +  i), 2*(n*j + i)+1, c);
    }
  }

  // Add a special source (knights)
  const vertex_desc v_source = boost::add_vertex(G);
  for(int kk=0; kk<k; kk++){
    int i, j; std::cin >> i >> j;
    adder.add_edge(v_source, 2*(n*j+i), 1);
  }
  
  // special case
  if(n*m*k == 0){
    std::cout << 0 << std::endl;
    return;
  }
  
  // Add a special sink (outside)
  const vertex_desc v_sink = boost::add_vertex(G);
  for(int i=0; i<n; i++){
    adder.add_edge(2*(n*0     + i)+1, v_sink, 1);
    adder.add_edge(2*(n*(m-1) + i)+1, v_sink, 1);
  }
  
  for(int j=0; j<m; j++){
    adder.add_edge(2*(n*j +   0)+1, v_sink, 1);
    adder.add_edge(2*(n*j + n-1)+1, v_sink, 1);
  }
  
  // Calculate flow from source to sink
  // The flow algorithm uses the interior properties (managed in the edge adder)
  // - edge_capacity, edge_reverse (read access),
  // - edge_residual_capacity (read and write access).
  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
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