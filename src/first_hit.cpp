#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

double round_down(const K::FT& x)
{
  double a = std::floor(CGAL::to_double(x));
  while(a > x) a-= 1;
  while(a+1 <= x) a += 1;
  return a;
}

void testcase(int n) {
  long x, y, a, b;
  std::cin >> x >> y >> a >> b;

  K::Point_2 start(x, y);
  K::Ray_2 ray(start, K::Point_2(a,b));
  
  std::vector<K::Segment_2> obstacles(n);
  for(int i=0; i<n; i++){
    long r, s, t, u;
    std::cin >> r >> s >> t >> u;
    
    K::Segment_2 obstacle(K::Point_2(r,s), K::Point_2(t,u));
    obstacles[i] = obstacle;
  }
  
  // last 20 points come from this
  std::random_shuffle(obstacles.begin(), obstacles.end());

  bool intersect = false;
  K::Segment_2 remaining;

  for(auto obstacle : obstacles){
    // first time
    if(!intersect){
      if(CGAL::do_intersect(ray, obstacle)){
        intersect = true;
        auto pt = CGAL::intersection(ray, obstacle);
        if(const K::Point_2* op = boost::get<K::Point_2>(&*pt)){
          remaining = K::Segment_2(start, *op);
        }
        else if(const K::Segment_2 *os = boost::get<K::Segment_2>(&*pt)){
          if (CGAL::collinear_are_ordered_along_line(start, os->source(), os->target())) {
            remaining = K::Segment_2(start, os->source());
          }
          else {
            remaining = K::Segment_2(start, os->target());
          }
        }
      }
    }
    else{
      if(CGAL::do_intersect(remaining, obstacle)){
        auto pt = CGAL::intersection(remaining, obstacle);
        if(const K::Point_2* op = boost::get<K::Point_2>(&*pt)){
          remaining = K::Segment_2(start, *op);
        }
        else if(const K::Segment_2 *os = boost::get<K::Segment_2>(&*pt)){
          if (CGAL::collinear_are_ordered_along_line(start, os->source(), os->target())) {
            remaining = K::Segment_2(start, os->source());
          }
          else {
            remaining = K::Segment_2(start, os->target());
          }
        }
      }
    }
  }

  if(!intersect) std::cout << "no" << std::endl;
  else std::cout << std::fixed // to make it not use scientific expression
                 << std::setprecision(0) // to make it not print after decimal point
                 << round_down(remaining.target().x()) << " " << round_down(remaining.target().y())
                 << std::endl;
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