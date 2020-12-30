#include <iostream>
#include <vector>
#include <map>

typedef std::vector<std::vector<int>> Matrix;

void testcase()
{
  int n; std::cin >> n;
  
  Matrix matrix;
  matrix.reserve(n);

  for(int i=0; i<n; i++){
    std::vector<int> preSumRow;
    for(int j=0; j<n; j++){
      int x; std::cin >> x;
      preSumRow.push_back((j==0) ? x & 1 : (x + preSumRow[j-1]) & 1);
    }
    matrix.push_back(preSumRow);
  }
  
  int cnt = 0;
  std::map<int, int> counter;
  for(int i=0; i<n; i++){
    for(int j=i; j<n; j++){
      counter = {{0,1}};
      int sum = 0;
      for(int k=0; k<n; k++){
        sum += matrix[k][j] - (i == 0 ? 0 : matrix[k][i-1]);
        sum = sum & 1;
        cnt += counter.find(sum) != counter.end() ? counter[sum] : 0;
        counter[sum]++;
      }
    }
  }
  
  std::cout << cnt << std::endl;
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
