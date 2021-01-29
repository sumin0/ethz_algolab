#include <iostream>
#include <vector>
#include <algorithm>

void testcase()
{
  int n; std::cin >> n;
  
  std::vector<int> ti(n);
  for(int i=0; i<n; ++i){
    std::cin >> ti[i];
  }
  
  for(int i=0; i<=(n-3)/2; ++i){
    // bomb has to be deactivated
    // before it explodes && one second before the top one explodes
    ti[2*i+1] = std::min(ti[2*i+1], ti[i]-1);
    ti[2*i+2] = std::min(ti[2*i+2], ti[i]-1);
  }
  
  std::sort(ti.begin(), ti.end());
  
  bool isPossible = true;
  for(int i=0; i<n; ++i){
    if(ti[i] <= i){
      isPossible = false;
      break;
    }
  }
  
  if(isPossible)
    std::cout << "yes\n";
  else
    std::cout << "no\n";
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
}