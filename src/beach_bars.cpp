#include <iostream>
#include <algorithm>
#include <vector>

void testcase()
{
  int n; std::cin >> n;
  
  std::vector<int> parasol;
  for(int i=0; i<n; i++){
    int x; std::cin >> x;
    parasol.push_back(x);
  }
  std::sort(parasol.begin(), parasol.end());
  
  std::vector<int> range(n, -1);
  int max = 0;
  for(int i=0; i<n; i++){
    int j = std::upper_bound(parasol.begin()+i, parasol.end(), 200 + parasol[i])
            - parasol.begin();
    
    range[i] = j;
    max = std::max(max, range[i]-i);
  }
  
  std::vector<int> dist(n, -1);
  int min = 100;
  for(int i=0; i<n; i++){
    if((range[i] - i) == max){
      dist[i] = (parasol[range[i]-1] - parasol[i] + 1)/2;
      min = std::min(min, dist[i]);
    }
  }
  
  std::vector<int> ans;
  for(int i=0; i<n; i++){
    if(dist[i] == min){
      if (((parasol[range[i]-1] + parasol[i]) & 1) == 1){
        if(parasol[range[i]-1] + parasol[i] < 0){
          ans.push_back((parasol[range[i]-1] + parasol[i] - 1)/2);
        }
        ans.push_back((parasol[range[i]-1] + parasol[i])/2);
        if(parasol[range[i]-1] + parasol[i] > 0){
          ans.push_back((parasol[range[i]-1] + parasol[i] + 1)/2);
        }
      }
      else ans.push_back((parasol[range[i]-1] + parasol[i])/2);
    }
  }
  
  std::cout << max << " " << min << std::endl;
  for(auto pos : ans) std::cout << pos << " ";
  std::cout << std::endl;
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