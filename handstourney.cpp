///4
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Triangulation_face_base_2.h>
#include <boost/pending/disjoint_sets.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <map>
#include <iomanip>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int count(std::map<int, int>& bins, int k) {
  if (k == 1) {
    return bins[1] + bins[2] + bins[3] + bins[4];
  } else if (k == 2) {
    return bins[2] + bins[3] + bins[4] + (bins[1] / 2);
  } else if (k == 3) {
    if (bins[2] > bins[1]) {
      return bins[3] + bins[4] + bins[1] + ((bins[2] - bins[1]) / 2);
    } else {
      return bins[3] + bins[4] + bins[2] + ((bins[1] - bins[2]) / 3);
    }
  } else { //k == 4
    int res = bins[4] + (bins[2] / 2);
    int rem = bins[2] % 2;
    
    if (bins[3] > bins[1]) {
      res += bins[1] + (bins[3] - bins[1] + rem) / 2;
      return res;
    } else { //bins[1] >= bins[3]
      res += bins[3] + (bins[1] - bins[3] + rem*2) / 4;
      return res;
    }
  }
}

void testcase() {
  int n, k, f_0; long s_0; std::cin >> n >> k >> f_0 >> s_0;
  std::vector<IPoint> points;
  for (int i = 0; i < n; i++) {
    int x, y; std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }

  Delaunay t;
  t.insert(points.begin(), points.end());

  EdgeV edges;
  edges.reserve(3*n); // there can be no more in a planar graph
  for (auto e = t.finite_edges_begin(); e != t.finite_edges_end(); ++e) {
    Index i1 = e->first->vertex((e->second+1)%3)->info();
    Index i2 = e->first->vertex((e->second+2)%3)->info();
    // ensure smaller index comes first
    if (i1 > i2) std::swap(i1, i2);
    edges.emplace_back(i1, i2, t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });
            
  std::map<int, int> bins; //bins[i] denotes # components of size i for 1 <= i <= 3
  //bins[4] denotes # components of size >= 4
  bins[1] = n;
  bins[2] = 0;
  bins[3] = 0;
  bins[4] = 0;

  std::map<int, int> comp_size; //comp_size[c_ind] = size of component with index c_ind in uf
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  
  int prev_count = n;
  int f = -1; K::FT s = -1;
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    K::FT curr_length = std::get<2>(*e);
    
    if (curr_length >= s_0 && f == -1) {
      f = prev_count;
    }
    
    int s1, s2;
    if (comp_size.count(c1) == 0) s1 = 1; 
    else s1 = comp_size[c1];
    
    if (comp_size.count(c2) == 0) s2 = 1; 
    else s2 = comp_size[c2];
    
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      int s3 = s1 + s2;
      Index c3 = uf.find_set(std::get<0>(*e));
      comp_size[c3] = s3;
      
      bins[std::min(4, s1)]--;
      bins[std::min(4, s2)]--;
      bins[std::min(4, s3)]++;
      prev_count = count(bins, k);
      
      if (prev_count < f_0 && s == -1) { //this is the first edge to cause count to drop below f_0
        s = curr_length;
      }
      
      if (--n_components == 1) break;
    }
  }
  if (f == -1) f = 1;
  
  std::cout << std::fixed << std::setprecision(0);
  std::cout << s << ' ' << f << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}