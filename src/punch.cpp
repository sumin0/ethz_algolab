#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

struct State{
  int cost; // total cost (first objective)
  int num; // total number of beverages (second objective)
  
  // for std::min operations
  bool operator<(const State &other) const{
    if(cost == other.cost)
      return num > other.num;
    return cost < other.cost;
  }
};

State recursive(std::vector<std::vector<State>> & dp,
              const std::vector<std::pair<int, int>> & b,
              const int i, const int j){
  
  if(i == 0 || j == 0){
    return dp[i][j];
  }
  if(j < 0) return {0, 0};
  
  if(dp[i][j].num != -1) return dp[i][j];
  
  // when not using the beverage i
  State s1 = recursive(dp, b, i-1, j);
  
  // when using one unit of beverage i
  State s2 = recursive(dp, b, i-1, j-b[i].second);
  s2.num += 1;
  s2.cost += b[i].first;
  
  // when using more than one unit of beverage i
  State s3 = recursive(dp, b, i, j-b[i].second);
  s3.cost += b[i].first;
  
  dp[i][j] = std::min(std::min(s1, s2), s3);
  return dp[i][j];
}

void testcase()
{
  int n, k; std::cin >> n >> k;
  
  std::vector<std::pair<int, int>> beverages(n);
  for(int i=0; i<n; ++i){
    int c, v; std::cin >> c >> v;
    beverages[i] = std::make_pair(c, v);
  }
  
  // dp table
  // row : beverages up to i are available
  // col : required amount of beverages (liters)
  std::vector<std::vector<State>> dp(n, std::vector<State>(k+1, {10000, -1}));
  for(int i=0; i<n; ++i){
    dp[i][0] = {0, 0};
  }
  for(int j=1; j<k+1; ++j){
    int cost = beverages[0].first * std::ceil(1.*j / beverages[0].second);
    dp[0][j] = {cost, 1};
  }
  
  State result = recursive(dp, beverages, n-1, k);
  std::cout << result.cost << " " << result.num << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
    
  return 0;
}