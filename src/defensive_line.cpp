#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <limits>

typedef std::vector<std::vector<int>> VVI;

int maxValue(const std::vector<int> &d, const std::vector<int> &m, VVI &memo,
            int start, int num_attack, int end)
{
  if(num_attack == 0) return 0;
  if(start >= end){
    return std::numeric_limits<int>::min();
  }
  
  if(m[start] != -1){
    // we take the interval
    int next_start = m[start] + 1;
    if(memo[next_start][num_attack-1] == -1)
      memo[next_start][num_attack-1] = maxValue(d, m, memo, next_start, num_attack-1, end);
      
    // we do not take the interval
    if(memo[start+1][num_attack] == -1)
      memo[start+1][num_attack] = maxValue(d, m, memo, start+1, num_attack, end);
      
    return std::max(memo[next_start][num_attack-1] + next_start - start,
                    memo[start+1][num_attack]);
  }
  else{
    if(memo[start+1][num_attack] == -1)
      memo[start+1][num_attack] = maxValue(d, m, memo, start+1, num_attack, end);
      
    return memo[start+1][num_attack];
  }
}

void testcase()
{
  int n, m, k;
  std::cin >> n >> m >> k;
  
  std::vector<int> defense(n, -1);
  for(int i=0; i<n; i++){
    std::cin >> defense[i];
  }
  
  // sliding window to find the interval that sums to k
  std::vector<int> intervals(n, -1);
  
  int j = 0;
  int sum = defense[0];
  for (int i=0; i<n; i++) {
    if (sum < k) {
      j++;
      if (j >= n) break;
      sum += defense[j];
      i--;
    }
    else if (sum == k) {
      // save (i, j) and continue from (i+1, j)
      intervals[i] = j;
      sum -= defense[i];
    }
    else { // sum > k
      sum -= defense[i];
    }
  }
  
  // dynamic programming
  VVI memo(n+1, std::vector<int>(m+1, -1));
  int ans = maxValue(defense, intervals, memo, 0, m, n);
  
  if(ans < 0) std::cout << "fail" << std::endl;
  else std::cout << ans << std::endl;
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
