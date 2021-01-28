#include <iostream>
#include <vector>
#include <algorithm>

const int INF = 500*1024+1;

int maxMoney(const std::vector<int> & v, std::vector<std::vector<int>> & memo,
             const int m, const int i, const int j)
{
  if(j < i) return 0; // invalid
  
  if(j - i < 1)
    return memo[i][j] = 0;
  
  if(memo[i][j] != -1)
    return memo[i][j];
  
  int left = INF, right = INF;
  for(int p=0; p<m; p++){
    left = std::min(left, maxMoney(v, memo, m, i+p+1, j-(m-p-1)));
    right = std::min(right, maxMoney(v, memo, m, i+p, j-(m-p)));
  }

  memo[i][j] = std::max(v[i] + left, v[j-1] + right);
  return memo[i][j];
}

void testcase()
{
  int n, m, k; std::cin >> n >> m >> k;
  std::vector<int> v;
  v.reserve(n);
  for(int i=0; i<n; i++){
    int v_i; std::cin >> v_i;
    v.push_back(v_i);
  }
  
  std::vector<std::vector<int>> memo(n+1, std::vector<int>(n+1, -1));
  
  // all scenarios for the first 'k' players
  int maximal_winning = INF;
  for(int i=0; i<=k; i++){
    maximal_winning = std::min(maximal_winning, maxMoney(v, memo, m, i, n - (k-i)));
  }

  std::cout << maximal_winning << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  for(int i=0; i<t; i++){
    testcase();
  }
}
