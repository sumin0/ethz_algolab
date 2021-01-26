#include <iostream>
#include <vector>
#include <algorithm>

struct Movement{
  long dist;
  long t;
  
  bool operator<(const Movement& other) const{
    return t < other.t;
  }
};

int n, m;
long D, T;

std::vector<Movement> moves;
std::vector<long> magicPotion;

void dfs(const int idx, const int end, const int gulps, const long traveled_dist, const long elapsed_t, std::vector<Movement> & half){
  
  if(elapsed_t >= T){
    return;
  }
  
  if(idx == end){
    if(traveled_dist > 0){
      half.push_back({traveled_dist, elapsed_t});
    }
    return;
  }
  
  // use the move
  dfs(idx + 1, end, gulps, traveled_dist + moves[idx].dist + magicPotion[gulps], elapsed_t + moves[idx].t, half);
  // do not use the move
  dfs(idx + 1, end, gulps, traveled_dist, elapsed_t, half);
  
}

bool isPossible(int gulps){
  // split
  std::vector<Movement> first_half;
  std::vector<Movement> second_half;
  
  // fill the lists
  dfs(0, n/2, gulps, 0, 0, first_half);
  dfs(n/2, n, gulps, 0, 0, second_half);
  
  // until the certain time, summarize the maximum distance
    // sort
  std::sort(first_half.begin(), first_half.end());
  std::sort(second_half.begin(), second_half.end());
    
    // replace values to the maximum distance
  for(int i=1; i<first_half.size(); ++i){
    first_half[i].dist = std::max(first_half[i].dist, first_half[i-1].dist);
  }
  for(int i=1; i<second_half.size(); ++i){
    second_half[i].dist = std::max(second_half[i].dist, second_half[i-1].dist);
  }
  
  // quick check before merging
  if(first_half.back().dist >= D || second_half.back().dist >= D)
    return true;
  
  // merge two lists
  int idx = second_half.size()-1;
  for(auto m : first_half){
    
    while(second_half[idx].t + m.t >= T)
      idx--;
    
    if(second_half[idx].dist + m.dist >= D)
      return true;
  }
  
  return false;
}

void testcase()
{
  // int n, m; long D, T;
  std::cin >> n >> m >> D >> T;

  moves.resize(n);
  for(int i=0; i<n; ++i){
    long d, t; std::cin >> d >> t;
    moves[i] = {d, t};
  }

  magicPotion.resize(m+1);
  for(int i=1; i<=m; i++){
    std::cin >> magicPotion[i];
  }
  
  // binary search
  int a = 0;
  int b = m+1;
  
  while(a != b){
    int gulps = (a+b)/2;
    
    if(isPossible(gulps)){
      b = gulps;
    }
    else{
      a = gulps + 1;
    }
  }
  
  if(a == (m+1)){
    std::cout << "Panoramix captured" << std::endl;
  }
  else{
    std::cout << a << std::endl;
  }
}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--){
    testcase();
  }
}
