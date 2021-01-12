#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

struct costTree{
  // case 1. root is selected
  int c1;
  // case 2. root is not selected; covered by a child
  int c2;
  // case 3. root is not selected; covered by a parent
  int c3;
  
  // case 1, 2: sub-tree is covered
  // case 3: not covered YET
};

costTree solve(const std::vector<std::vector<int>> &children,
               const std::vector<int> &costs,
               int root)
{
  if(children[root].size() == 0)
    return {costs[root], costs[root], 0};
  
  costTree cost_root = {costs[root], 0, 0};
  int min_child_cost = std::numeric_limits<int>::max();
  for(auto child : children[root]){
    costTree cost_child = solve(children, costs, child);
    
    cost_root.c1 += std::min(cost_child.c1, cost_child.c3); // min of all three values, c2 is always bigger than c3
    cost_root.c3 += std::min(cost_child.c1, cost_child.c2); // child should be covered
    
    min_child_cost = std::min(min_child_cost,
                               cost_child.c1 - std::min(cost_child.c1, cost_child.c2));
  }
  // cost_root.c2
  // = min_j (child_j.c1 + sum_{i!=j}_{min(child_i.c1, child_i.c2)})
  // = min_j (sum_{all i}_{min(child_i.c1, child_i.c2)} - min(child_j.c1, child_j.c2) + child_j.c1)
  // = min_j (cost_root.c3 - min(child_j.c1, child_j.c2) + child_j.c1)
  
  cost_root.c2 = cost_root.c3 + min_child_cost;
  
  return cost_root;
}

void testcase()
{
  int n; std::cin >> n;
  
  std::vector<std::vector<int>> child(n);
  for(int i=0; i<n-1; i++){
    int u, v; std::cin >> u >> v;
     
    child[u].push_back(v);
  }
  
  std::vector<int> cost(n);
  for(int i=0; i<n; i++){
    int c; std::cin >> c;
    cost[i] = c;
  }

  costTree total = solve(child, cost, 0);
  std::cout << std::min(total.c1, total.c2) << std::endl;
  
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