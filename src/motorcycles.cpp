#include <iostream>
#include <vector>
#include <algorithm>
#include <CGAL/Gmpq.h>

struct Biker{
  int idx;
  long start_y;
  CGAL::Gmpq slope;
  
  Biker() {}
  Biker(int i, long y0, long x1, long y1) : idx(i), start_y(y0) {
    slope = CGAL::Gmpq(y1-y0, x1);
  }
};

void testcase() {
  int n; std::cin >> n;
  
  std::vector<Biker> bikers(n);
  for(int i=0; i<n; i++){
    long y0, x1, y1;
    std::cin >> y0 >> x1 >> y1;
    bikers[i] = Biker(i, y0, x1, y1);
  }
  
  // sort
  std::sort(bikers.begin(), bikers.end(), [](const Biker &b1, const Biker &b2){
    return b1.start_y < b2.start_y;
  });
  

  std::vector<bool> riding(n, true);
  // two iterations instaed of nested for loops!
  // iterate from bottom to top
  Biker mighty_biker = bikers.front();
  for(int i=1; i<n; i++){
    if((CGAL::abs(bikers[i].slope) < CGAL::abs(mighty_biker.slope)) ||
      ((CGAL::abs(bikers[i].slope) == CGAL::abs(mighty_biker.slope)) && (bikers[i].slope > mighty_biker.slope)))
    {
      mighty_biker = bikers[i];
    }
    else if(bikers[i].slope < mighty_biker.slope){ // intersects! biker i stops
      riding[bikers[i].idx] = false;
    }
  }
  
  // iterate from top to bottom
  mighty_biker = bikers.back();
  for(int i=n-2; i>=0; i--){
    if(CGAL::abs(bikers[i].slope) <= CGAL::abs(mighty_biker.slope)){
      mighty_biker = bikers[i];
    }
    else if(bikers[i].slope > mighty_biker.slope){
      riding[bikers[i].idx] = false;
    }
  }
  
  for(int i=0; i<n; i++){
    if(riding[i])
      std::cout << i << " ";
  }
  std::cout << std::endl;

}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  int t; std::cin >> t;
  while(t--){
    testcase();
  }
}