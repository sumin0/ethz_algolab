#include <iostream>

void testcase()
{
  long n; std::cin >> n;
  
  long max = 0;
  for(int i=0; i<n; i++){
    long h; std::cin >> h;
    if((i <= max) && (max < i+h-1)) max = i+h-1;
  }
  
  ++max;
  max = (max < n) ? max : n;
  std::cout << max << std::endl;
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
