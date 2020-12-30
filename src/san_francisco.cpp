#include <iostream>
#include <vector>
#include <algorithm>

struct Canal{
  int to;
  long long point;
  bool isMove;
  
  Canal(int v, long long p, bool b) : to(v), point(p), isMove(b) {};
};

long long maxPoint(int from, int moves, const std::vector<std::vector<Canal>> &canals, std::vector<std::vector<long long>> &memo)
{
  if(moves == 0) return 0;
  
  if(memo[from][moves] == -1){
    long long pt = 0;
    for(auto c : canals[from]){
      if(c.isMove)
        pt = std::max(pt, c.point + maxPoint(c.to, moves-1, canals, memo));
      else
        pt = std::max(pt, c.point + maxPoint(c.to, moves, canals, memo));
    }
    memo[from][moves] = pt;
  }
  
  return memo[from][moves];
}

void testcase()
{
  int numHoles, numCanals, maxMoves;
  long long goalScore;
  std::cin >> numHoles >> numCanals >> goalScore >> maxMoves;

  std::vector<std::vector<Canal>> canals(numHoles);
  for(int i=0; i<numCanals; i++){
    int u, v; long long p;
    std::cin >> u >> v >> p;
    canals[u].push_back(Canal(v, p, true));
  }
  
  for(int i=0; i<numHoles; i++){
    if(canals[i].size() == 0){
      canals[i].push_back(Canal(0, 0, false));
    }
  }
  
  std::vector<std::vector<long long>> memo(numHoles, std::vector<long long>(maxMoves+1, -1));
  
  bool possible = false;
  int moves;
  for(moves=1; moves<=maxMoves; moves++){
    long long max_score = maxPoint(0, moves, canals, memo);
    // std::cout << "with " << moves << " moves: " << max_score << "\n";
    if(max_score >= goalScore){
      possible = true;
      break;
    }
  }
  
  if(possible)
    std::cout << moves << std::endl;
  else
    std::cout << "Impossible" << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--){
    testcase();
  }

  return 0;
}
