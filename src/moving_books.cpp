#include <iostream>
#include <vector>
#include <algorithm>

void testcase()
{
  int n, m;
  std::cin >> n >> m;
  
  std::vector<int> strength(n);
  for(int i=0; i<n; ++i){
    std::cin >> strength[i];
  }
  
  std::vector<int> book(m);
  for(int i=0; i<m; ++i){
    std::cin >> book[i];
  }
  
  std::sort(strength.begin(), strength.end(), std::greater<>());
  std::sort(book.begin(), book.end(), std::greater<>());
  
  if(strength[0] < book[0]){
    std::cout << "impossible\n";
    return;
  }
  
  int l = (m-1)/n + 1;
  int u = m;
  while(l < u){
    int numBook = (l+u)/2;
    
    bool isPossible = true;
    for(int i=0; i<(m-1)/numBook+1; i++){
      if(strength[i] < book[i*numBook]){
        isPossible = false;
        break;
      }
    }
    
    if(isPossible)
      u = numBook;
    else
      l = numBook+1;
  }
  
  std::cout << l*3-1 << std::endl;
  
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--)
    testcase();
  
  return 0;
}