#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

std::vector<std::pair<int, int>> path;
std::vector<int> result;

int find(int max_age){
  // using lower_bound function
  auto p = std::lower_bound(std::begin(path), std::end(path), max_age, [](const auto &pair, const int value){
    return pair.second > value;
  });
  
  return p->first;
}

void dfs(const std::vector<std::vector<std::pair<int, int>>> &queries,
         const std::vector<std::vector<int>> &children,
         const std::vector<int> &age,
         const int u){
  
  path.emplace_back(u, age[u]);
  
  for(auto q : queries[u]){
    result[q.first] = find(q.second);
  }
  
  for(int child : children[u]){
    dfs(queries, children, age, child);
  }
  
  path.pop_back();
}

void testcase()
{
  int n, q; std::cin >> n >> q;
  
  std::map<std::string, int> id_map; // name to id
  std::vector<int> age(n);
  std::vector<std::string> name(n); // id to name
  
  for(int i=0; i<n; ++i){
    std::cin >> name[i] >> age[i];
    id_map[name[i]] = i;
  }
  
  std::vector<std::vector<int>> children(n);
  for(int i=0; i<n-1; ++i){
    std::string s, p;
    std::cin >> s >> p;
    children[id_map[p]].push_back(id_map[s]);
  }
  
  // queries
  // store in vector, key = query species
  // store as pair: (query idx, max age)
  std::vector<std::vector<std::pair<int, int>>> queries(n);
  for(int i=0; i<q; ++i){
    std::string s; int b;
    std::cin >> s >> b;
    queries[id_map[s]].push_back({i, b});
  }
  
  result.resize(n);
  path.clear();
  int root = std::max_element(age.begin(), age.end()) - age.begin();
  dfs(queries, children, age, root);
  
  for(int i=0; i<q; ++i){
    std::cout << name[result[i]] << " ";
  }
  std::cout << "\n";
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