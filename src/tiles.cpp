#include <iostream>
#include <vector>
// #include <algorithm>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
// #include <boost/graph/connected_components.hpp>

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
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};


void testcase()
{
  int w, h;
  std::cin >> w >> h;
  
  graph G(w*h);
  edge_adder adder(G);
  
  const vertex_desc v_source = boost::add_vertex(G);
  const vertex_desc v_sink = boost::add_vertex(G);
  
  std::vector<std::vector<char>> tiles(h, std::vector<char>(w));
  int odd_count = 0, even_count = 0;
  for(int i=0; i<h; i++){
    for(int j=0; j<w; j++){
      char tmp; std::cin >> tmp;
      if(tmp == '.'){
        if((i+j) & 0x01){
          adder.add_edge(v_source, w*i + j, 1);
          tiles[i][j] = 'o';
          ++odd_count;
        }
        else{
          adder.add_edge(w*i + j, v_sink, 1);
          tiles[i][j] = 'e';
          ++even_count;
        }
      }
      else
        tiles[i][j] = 'x';
    }
  }
  
  for(int i=0; i<h-1; i++){
    for(int j=0; j<w; j++){
      if((tiles[i][j] == 'o') && (tiles[i+1][j] == 'e'))
        adder.add_edge(w*i + j, w*(i+1) + j, 1);
      else if ((tiles[i][j] == 'e') && (tiles[i+1][j] == 'o'))
        adder.add_edge(w*(i+1) + j, w*i + j, 1);
    }
  }
  
  for(int i=0; i<h; i++){
    for(int j=0; j<w-1; j++){
      if((tiles[i][j] == 'o') && (tiles[i][j+1] == 'e'))
        adder.add_edge(w*i + j, w*i + (j+1), 1);
      else if ((tiles[i][j] == 'e') && (tiles[i][j+1] == 'o'))
        adder.add_edge(w*i + (j+1), w*i + j, 1);
    }
  }
  
  // sanity check
  if(odd_count != even_count){
    std::cout << "no\n";
    return;
  }

  long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
  
  if(flow == odd_count)
    std::cout << "yes\n";
  else
    std::cout << "no\n";
  
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
