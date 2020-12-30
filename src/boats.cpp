#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct Boat{
  int len;
  int pos;
};

void testcase()
{
  int n; std::cin >> n;
  std::vector<Boat> boats(n);
  for(int i=0; i<n; i++){
    Boat tmp; std::cin >> tmp.len >> tmp.pos;
    boats[i] = tmp;
  }
  
  std::sort(boats.begin(), boats.end(),
            [](const Boat& b1, const Boat& b2) -> bool{
              return b1.pos < b2.pos;
            });
            
  int count = 0;
  int curr_pos = std::numeric_limits<int>::min();
  int prev_pos = std::numeric_limits<int>::min();
  for(int i=0; i<n; i++){
    if(curr_pos > boats[i].pos){
      curr_pos = std::min(curr_pos, std::max(boats[i].pos, prev_pos + boats[i].len));
    }
    else{
      prev_pos = curr_pos;
      curr_pos = std::max(boats[i].pos, curr_pos + boats[i].len);
      count++;
    }
  }
  
  std::cout << count << std::endl;

}

int main(){
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--){
    testcase();
  }

  return 0;
}