#include <iostream>

void testcase()
{
  int n; std::cin >> n;
  
  int cntOdd = 0;
  int sum = 0;
  for(int i=0; i<n; i++){
    int x; std::cin >> x;
    sum += x;
    sum = sum & 1;
    if(sum) cntOdd++;
  }
  
  int cntEven = n - cntOdd;
  int answer = cntEven*(cntEven-1)/2
             + cntOdd*(cntOdd-1)/2
             + cntEven;
  
  std::cout << answer << std::endl;
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
