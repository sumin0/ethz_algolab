#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

typedef std::pair<int, int> PII;

void testcase()
{
  int n; std::cin >> n;
  
  long long N = 0;
  std::vector<int> m(n, -1);
  for(int i=0; i<n; i++){
    std::cin >> m[i];
    N += m[i];
  }
  
  std::vector<PII> p;
  p.reserve(N);
  for(int i=0; i<n; i++){
    for(int j=0; j<m[i]; j++){
      int tmp_p; std::cin >> tmp_p;
      p.push_back(std::make_pair(i, tmp_p));
    }
  }
  
  std::sort(p.begin(), p.end(),
            [](const PII& p1, const PII& p2) -> bool {
              return (p1.second < p2.second);
            });
  
  long long min_length = p[N-1].second - p[0].second + 1;
  std::map<int, int> counter;
  int i = 0;
  for (int j=0; j<N; j++) {
    int idx = p[j].first;
    counter[idx] = (counter.count(idx)) ? counter[idx] + 1 : 1;
    
    while (counter[p[i].first] > 1){
      counter[p[i].first]--;
      i++;
    }
    
    if (int(counter.size()) == n) {
      // save (i, j) and continue from (i+1, j)
      long long length = p[j].second - p[i].second + 1;
      if (min_length > length){
        min_length = length;
      }
    }
  }
  
  std::cout << min_length << std::endl;
  
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