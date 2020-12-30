#include <iostream>
#include <vector>
// #include <set>
#include <string>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int common(const std::vector<std::string> &s1, const std::vector<std::string> &s2){
  int i1 = 0, i2 = 0;
  int common = 0;
  
  while(i1 < s1.size() && i2 < s2.size()){
    if(s1[i1] == s2[i2]){
      common++;
      i1++; i2++;
    }
    else if(s1[i1] < s2[i2]) i1++;
    else i2++;
  }
  return common;
}

void testcase() {
  int n, c, f;
  std::cin >> n >> c >> f;
  
  // vector + sort instead of set
  std::vector<std::vector<std::string>> characteristics(n);
  for(int i=0; i<n; i++){
    characteristics[i].resize(c);
    for(int j=0; j<c; j++){
      std::cin >> characteristics[i][j];
    }
    std::sort(characteristics[i].begin(), characteristics[i].end());
  }
  
  // yet need to write a function that computes number of common characteristics
  Graph G(n);
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (common(characteristics[i], characteristics[j]) > f) {
        boost::add_edge(i, j, G);
      }
    }
  }
  
  std::vector<Vertex> buddy(n);
  edmonds_maximum_cardinality_matching(G, &buddy[0]);
  if ((int)matching_size(G, &buddy[0]) == (n/2))
    std::cout << "not optimal" << std::endl;
  else
    std::cout << "optimal" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--) {
    testcase();
  }

  return 0;
}