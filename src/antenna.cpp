#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Min_circle_2_traits_2<K> K_Traits;
typedef CGAL::Min_circle_2<K_Traits> K_Min_circle;
typedef CGAL::Min_circle_2_traits_2<IK> IK_Traits;
typedef CGAL::Min_circle_2<IK_Traits> IK_Min_circle;

double round_up(const K::FT& x)
{
  double a = std::ceil(CGAL::to_double(x));
  while(a < x) a += 1;
  while(a >= x+1) a -= 1;
  return a;
}

void testcase(int n)
{
  std::vector<IK::Point_2> points;
  points.reserve(n);
  for(int i=0; i<n; i++){
    long x, y; std::cin >> x >> y;
    points.push_back(IK::Point_2(x, y));
  }
  
  IK_Min_circle mc(points.begin(), points.end(), true);
  
  // to satisfy the time restriction,
  // first create a circle with inexact kernel and get the support points (which are integer)
  // then create a circle with exact kernel using only the support points to get the exact radius
  std::vector<K::Point_2> new_points;
  new_points.reserve(mc.number_of_support_points());
  for(auto it=mc.support_points_begin(); it!=mc.support_points_end(); ++it){
    long x = it->x(); long y = it->y();
    new_points.push_back(K::Point_2(x, y));
  }
  
  K_Min_circle new_mc(new_points.begin(), new_points.end(), true);
  K_Traits::Circle c = new_mc.circle();
  
  K::FT radius = CGAL::sqrt(c.squared_radius());
  
  std::cout << std::fixed // to make it not use scientific expression
            << std::setprecision(0) // to make it not print after decimal point
            << round_up(radius)
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
