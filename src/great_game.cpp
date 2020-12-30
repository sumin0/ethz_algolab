#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <limits>

int minMove(const std::multimap<int, int> & transition, const int target,
            int currentPos, std::vector<int> & memoMinMove, std::vector<int> & memoMaxMove);
            
int maxMove(const std::multimap<int, int> & transition, const int target,
            int currentPos, std::vector<int> & memoMinMove, std::vector<int> & memoMaxMove);

int minMove(const std::multimap<int, int> & transition, const int target,
            int currentPos, std::vector<int> & memoMinMove, std::vector<int> & memoMaxMove)
{
  if(currentPos == target) return 0;
  
  int ans = std::numeric_limits<int>::max();
  auto range = transition.equal_range(currentPos);
  for(auto it = range.first; it != range.second; it++){
    if(memoMinMove[it->second] == -1){
      memoMinMove[it->second] = maxMove(transition, target, it->second, memoMinMove, memoMaxMove);
    }
    ans = std::min(ans, memoMinMove[it->second]);
  }
  
  return 1 + ans;
}

int maxMove(const std::multimap<int, int> & transition, const int target,
            int currentPos, std::vector<int> & memoMinMove, std::vector<int> & memoMaxMove)
{
  if(currentPos == target) return 0;
  
  int ans = -1;
  auto range = transition.equal_range(currentPos);
  for(auto it = range.first; it != range.second; it++){
    if(memoMaxMove[it->second] == -1){
      memoMaxMove[it->second] = minMove(transition, target, it->second, memoMinMove, memoMaxMove);
    }
    ans = std::max(ans, memoMaxMove[it->second]);
  }
  
  return 1 + ans;
}

void testcase()
{
  int n, m; std::cin >> n >> m;
  int r, b; std::cin >> r >> b;
  
  std::multimap<int, int> transition;
  for(int i=0; i<m; i++){
    int u, v; std::cin >> u >> v;
    transition.insert(std::make_pair(u, v));
  }
  
  std::vector<int> memoMinMove(n+1, -1);
  std::vector<int> memoMaxMove(n+1, -1);
  
  int Sherlock = minMove(transition, n, r, memoMinMove, memoMaxMove);
  int Moriarty = minMove(transition, n, b, memoMinMove, memoMaxMove);
  
  int ans;
  if(Sherlock < Moriarty) ans = 0;
  else if(Sherlock == Moriarty){
    if(Sherlock % 2 == 0) ans = 1;
    else ans = 0;
  }
  else ans = 1;
  
  std::cout << ans << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for(int i=0; i<t; i++){
    testcase();
  }

  return 0;
}