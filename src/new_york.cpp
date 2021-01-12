#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <algorithm>

void dfs(const std::vector<int> &temp,
         const std::vector<int> &parent,
         std::vector<bool> &visited,
         std::vector<bool> &safe,
         const int k,
         const int m,
         int idx)
{
  std::queue<int> path;
  std::multiset<int> temps;
  
  // find a path that ends up at idx
  path.push(idx);
  temps.insert(temp[idx]);
  
  // true means that it has been considered as the leaf node of a path already
  visited[idx] = true;
  
  int p_idx = idx;
  int max_t, min_t;
  while(true){
    p_idx = parent[p_idx];
    if(p_idx == -1) return;
    
    path.push(p_idx);
    temps.insert(temp[p_idx]);
    
    max_t = *(temps.rbegin());
    min_t = *temps.begin();
    
    if((path.size() == m) && (max_t - min_t <= k)){
      safe[p_idx] = true;
      visited[idx] = true;
      
      path.pop();
      temps.erase(temps.find(temp[idx]));
      idx = parent[idx];
      
      if(visited[idx]) return;
    }
    else if(max_t - min_t > k){
      visited[idx] = true;
      path.pop();
      temps.erase(temps.find(temp[idx]));
      idx = parent[idx];
      
      if(visited[idx]) return;
    }
    
  }
  
}


void testcase(){
  int n, m, k;
  std::cin >> n >> m >> k;
  
  std::vector<int> temp(n);
  for(int i=0; i<n; ++i){
    int hi; std::cin >> hi;
    
    temp[i] = hi;
  }
  
  // each node has only one parent!
  std::vector<int> parent(n, -1);
  std::vector<bool> node(n, true);
  for(int i=0; i<n-1; ++i){
    int u, v; std::cin >> u >> v;
    
    parent[v] = u;
    node[u] = false;
  }
  
  std::vector<bool> safe(n, false);
  std::vector<bool> visited(n, false);
  
  // DFS
  for(int i=0; i<n; ++i){
    if(node[i])
      dfs(temp, parent, visited, safe, k, m, i);
  }
  
  bool feasible = false;
  for(int i=0; i<n; ++i){
    if(safe[i]){
      std::cout << i << " ";
      feasible = true;
    }
  }
  if(!feasible) std::cout << "Abort mission";
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