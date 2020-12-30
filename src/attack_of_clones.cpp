#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

struct Segment{
  int start, end;
  Segment(int a, int b) : start(a), end(b) {};
};

void testcase()
{
  int n, m; std::cin >> n >> m;
  std::vector<Segment> segs;
  segs.reserve(n);

  // to not get assertion error, count in a smart way
  std::map<int, int> counter;
  
  for(int i=0; i<n; i++){
    int a, b; std::cin >> a >> b;
    a--; b--;
    segs.push_back(Segment(a, b));
    
    counter[a]++;
    counter[b]--;
    if(a > b) counter[0]++;
  }
  
  // need to find where to start
  // should be the one with the least overlap
  int min_disk = 0;
  int min_overlaps = 11;
  int overlaps = 0;
  for (auto c : counter) {
    overlaps += c.second;
    if (overlaps < min_overlaps) {
      min_disk = c.first;
      min_overlaps = overlaps;
    }
  }
  min_disk++;

  // shift
  for(int i=0; i<n; i++){
    segs[i].start = (segs[i].start - min_disk + m) % m;
    segs[i].end = (segs[i].end - min_disk + m) % m;
    
    if(segs[i].start > segs[i].end) segs[i].end += m;
  }
  
  // sort by end time
  std::sort(segs.begin(), segs.end(),
            [](const Segment& s1, const Segment& s2) -> bool{
              return s1.end < s2.end;
            });
  
  // find the maximum
  int count = 0;
  int curr_pos = -1;
  for(auto seg : segs){
    if(((seg.start <= curr_pos) && (curr_pos <= seg.end)) || ((seg.end >= m) && (seg.end % m >= segs[0].start))){
      curr_pos = std::min(curr_pos, seg.end);
    }
    else{
      curr_pos = seg.end;
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
