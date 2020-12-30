#include <iostream>
#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <utility>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 P;

class Triangle {
  P p0, p1, p2, p3, p4, p5;
public:
  Triangle(P t0, P t1, P t2, P t3, P t4, P t5) {
    p0 = t0;
    p1 = t1;
    p2 = t2;
    p3 = t3;
    p4 = t4;
    p5 = t5;
  }
  
public:
  bool isPointInside(P &q) {
  return !CGAL::right_turn(this->p0, this->p1, q)
      && !CGAL::right_turn(this->p2, this->p3, q)
      && !CGAL::right_turn(this->p4, this->p5, q);
  }
};

void testcase()
{
  int m, n; std::cin >> m >> n;
  
  std::vector<P> path(m);
  for(int i=0; i<m; i++){
    int x, y; std::cin >> x >> y;
    path[i] = P(x, y);
  }
  
  std::vector<std::vector<bool>> map(n, std::vector<bool>(m-1, false));
  for (int i = 0; i < n; i++) {
    std::vector<P> tri(6);
    for (int j = 0; j < 6; j++) {
      int x, y; std::cin >> x >> y;
      tri[j] = P(x, y);
    }
    
    if (CGAL::right_turn(tri[0], tri[1], tri[2]))
      std::swap(tri[0], tri[1]);
    if (CGAL::right_turn(tri[2], tri[3], tri[4]))
      std::swap(tri[2], tri[3]);
    if (CGAL::right_turn(tri[4], tri[5], tri[0]))
      std::swap(tri[4], tri[5]);
    
    Triangle triangle(tri[0], tri[1], tri[2], tri[3], tri[4], tri[5]);
    
    for(int j=0; j<m-1; j++){
      if(triangle.isPointInside(path[j]) && triangle.isPointInside(path[j+1]))
        map[i][j] = true;
    }
  }
  
  // sliding window
  int min_cost = n;
  std::vector<int> counter(m-1, 0);
  int i = 0;
  for (int j=0; j<n; j++) {
    
    for (int k=0; k<m-1; k++){
      if (map[j][k]) counter[k]++;
    }

    bool can = true;
    while (i <= j){
      for(int k=0; k<m-1; k++){
        if (map[i][k] && (counter[k] == 1)){
          can = false;
          break;
        }
      }
      if(can){
        for(int k=0; k<m-1; k++){
          if (map[i][k]) counter[k]--;
        }
        i++;
      }
      else break;
    }
    
    bool finish = true;
    for(int k=0; k<m-1; k++){
      if (counter[k] == 0) finish = false;
    }
    if(finish){
      int cost = j - i + 1;
      if (min_cost > cost){
        min_cost = cost;
      }
    }
  }
  
  std::cout << min_cost << std::endl;
  
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