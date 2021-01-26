#include <iostream>
#include <vector>
#include <map>

class Queue{
public:
  Queue(int a, int b, int c) : x(a), y(b), z(c) {}
  
  Queue(int key) : x(key & 15), y((key >> 4) & 15), z(key >> 8) {}

  int get_numType(int m){
    return (m == 3) ? ((z && z!=y && z!=x) + (y && y!=x) + 1) : ((y && y!=x) + 1);
  }
  
  Queue push(int new_fighter){
    return Queue(new_fighter, x, y);
  }
  
  int get_key(){
    return x + (y << 4); // do not store the last one and save time
  }
  
private:
  int x, y, z;
  
};

struct State{
  int diff; // possible range: -11 to 11
  Queue south, north;
  
  int get_key(){
    return ((diff + 12) << 16) + (south.get_key() << 8) + (north.get_key());
  }
  
  int get_excitement(bool sud, int m){
    int numType = sud ? south.get_numType(m) : north.get_numType(m);
    return 1000 * numType - (1 << std::abs(diff));
  }
};

State get_state(int key){
  int north = key & 255;
  int south = (key >> 8) & 255;
  int diff = (key >> 16) - 12;
  
  return {diff, Queue(south), Queue(north)};
}

void testcase()
{
  int n, k, m;
  std::cin >> n >> k >> m;
  
  std::vector<int> fighters(n);
  for(int i=0; i<n; ++i){
    int f; std::cin >> f;
    fighters[i] = ++f;
  }
  
  State init = {0, Queue(0,0,0), Queue(0,0,0)};
  
  std::map<int, int> current_states;
  current_states[init.get_key()] = 0;
  for(int f : fighters){
    std::map<int, int> next_states;
    for(auto s : current_states){
      State state = get_state(s.first);
      
      State s_south = {state.diff + 1, state.south.push(f), state.north};
      State s_north = {state.diff - 1, state.south, state.north.push(f)};
      
      int excite_south = s_south.get_excitement(true, m);
      int excite_north = s_north.get_excitement(false, m);
      
      if(excite_south >= 0){
        next_states[s_south.get_key()] = std::max(excite_south + s.second,
          next_states[s_south.get_key()]);
      }
      if(excite_north >= 0){
        next_states[s_north.get_key()] = std::max(excite_north + s.second,
          next_states[s_north.get_key()]);
      }
    }
    current_states = next_states;
  }
  
  int max_excite = 0;
  for(auto s : current_states){
    max_excite = std::max(s.second, max_excite);
  }
  
  std::cout << max_excite << std::endl;
  
}

int main()
{
  int t; std::cin >> t;
  
  while(t--)
    testcase();
    
  return 0;
}