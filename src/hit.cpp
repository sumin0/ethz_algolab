#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

void testcase(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;

  K::Ray_2 ray(K::Point_2(x,y), K::Point_2(a,b));

  bool intersect = false;

  while(n--) {
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;
    if(intersect) continue;

    K::Segment_2 obstacle(K::Point_2(r,s), K::Point_2(t,u));

    intersect = intersect || CGAL::do_intersect(ray, obstacle);
  }

  if(intersect) std::cout << "yes" << std::endl;
  else std::cout << "no" << std::endl;
}

int main()
{
  std::ios_base::sync_with_stdio(false);
  
  while(true){
    int n; std::cin >> n;
    if(n == 0) break;
    
    testcase(n);
  }

  return 0;
}
