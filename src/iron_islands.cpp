#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

void testcase()
{
  int n, k, w;
  std::cin >> n >> k >> w;
  
  std::vector<int> numMen(n);
  for(int i=0; i<n; ++i){
    std::cin >> numMen[i];
  }
  
  std::vector<int> len(w);
  std::vector<std::vector<int>> waterway(w);
  for(int i=0; i<w; ++i){
    std::cin >> len[i];
    waterway[i].reserve(len[i]);
    
    for(int j=0; j<len[i]; ++j){
      int r; std::cin >> r;
      waterway[i].push_back(r);
    }
  }
  
  // within a single waterway
  // sliding window
  int maxNum = 0;
  for(int i=0; i<w; ++i){
    int a = 0;
    int b = 1;
    
    int sum = numMen[waterway[i][0]];
    
    while(a < len[i] && b <= len[i]){
      if(sum == k){
        maxNum = std::max(maxNum, b-a);
      }
      
      if(sum >= k || b == len[i]){
        sum -= numMen[waterway[i][a++]];
      }
      else{
        sum += numMen[waterway[i][b++]];
      }
    }
  }
  
  std::map<int, int> all_sums;
  for(int i=0; i<w; ++i){
    std::vector<int> sums;
    sums.push_back(0);
    for(int j=1; j<len[i]; ++j){
      int sum = sums[j - 1] + numMen[waterway[i][j]];
      
      if(sum + numMen[0] >= k) break;
      
      sums.push_back(sum);

      // find a match from the past waterways
      int target = k - sum - numMen[0];
      auto match = all_sums.find(target);
      if(match != all_sums.end()) {
         maxNum = std::max(maxNum, j + match->second + 1);
      }
    }
    
    // update all_sums
    for (int j=1; j<sums.size(); ++j){
      all_sums[sums[j]] = std::max(j, all_sums[sums[j]]);
    }
  }
  
  std::cout << maxNum << "\n";
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
}