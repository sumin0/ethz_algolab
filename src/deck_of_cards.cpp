#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>

void testcase()
{
  int n, k; std::cin >> n >> k;
  
  std::vector<int> v;
  v.reserve(n+1);
  v.push_back(0);
  for(int i=1; i<=n; i++){
    int v_i; std::cin >> v_i;
    v.push_back(v_i +v[i-1]);
  }

  // v is a sorted sequence since v_i are positive
  int opt_score = k;
  int opt_i = 0; int opt_j = 0;
  for(int i=0; i<n; i++){
    int j = std::lower_bound(v.begin() + i, v.end(), k + v[i]) - v.begin();
    
    int lower_score = k + v[i] - v[j-1];
    int upper_score = std::abs(v[std::min(j, n)] - (k + v[i]));
    
    int score = (lower_score <= upper_score) ? lower_score : upper_score;
    
    if(score < opt_score){
      opt_score = score;
      opt_i = i;
      opt_j = (lower_score <= upper_score) ? j-2 : j-1;
    }
  }
  
  std::cout << opt_i << " " << opt_j << std::endl;
  
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