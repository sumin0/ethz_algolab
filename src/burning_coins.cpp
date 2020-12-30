#include <iostream>
#include <vector>
#include <algorithm>

int maxMoney(const std::vector<int> & v, std::vector<std::vector<int>> & m, int i, int j)
{
  if(i == j) return v[i];
  if(i+1 == j) return std::max(v[i], v[j]);
  
  if(m[i+2][j] == -1)
    m[i+2][j] = maxMoney(v, m, i+2, j);

  if(m[i+1][j-1] == -1)
    m[i+1][j-1] = maxMoney(v, m, i+1, j-1);

  if(m[i][j-2] == -1)
    m[i][j-2] = maxMoney(v, m, i, j-2);

  return std::max(v[i] + std::min(m[i+2][j], m[i+1][j-1]),
                  v[j] + std::min(m[i+1][j-1], m[i][j-2]));
  
}

void testcase()
{
  int n; std::cin >> n;
  std::vector<int> v;
  v.reserve(n);
  for(int i=0; i<n; i++){
    int v_i; std::cin >> v_i;
    v.push_back(v_i);
  }
  
  std::vector<std::vector<int>> m(n, std::vector<int>(n, -1));
  
  int ans = maxMoney(v, m, 0, n-1);
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
