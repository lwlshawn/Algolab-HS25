// ETH AlgoLab example code: Compute a Euclidean minimum spanning tree (EMST)
// for n points p_0,...,p_{n-1} in O(n log n) time. Output the edges as ordered
// pairs of vertex indices (smaller first) together with the squared length; for
// instance, an edge between p_4=(0,0) and p_2=(1,2) is printed as "2 4 5".

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

// Epic kernel is enough, no constructions needed, provided the squared distance
// fits into a double (!)
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
// we want to store an index with each vertex
typedef std::size_t                                            Index;
typedef CGAL::Triangulation_vertex_base_with_info_2<Index,K>   Vb;
typedef CGAL::Triangulation_face_base_2<K>                     Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>            Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>                  Delaunay;

// As edges are not explicitly represented in the triangulation, we extract them
// from the triangulation to be able to sort and process them. We store the
// indices of the two endpoints, first the smaller, second the larger, and third
// the squared length of the edge. The i-th entry, for i=0,... of a tuple t can
// be accessed using std::get<i>(t).
typedef std::tuple<Index,Index,K::FT> Edge;
typedef std::vector<Edge> EdgeV;
typedef std::pair<K::Point_2,Index> IPoint;


std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int solve(std::vector<IPoint>& points, int k, int n, long r) {
  Delaunay t;
  auto it = points.begin();
  it = std::next(it, k);
  t.insert(it, points.end());

  // extract edges and sort by (squared) length
  // This step takes O(n log n) time (for the sorting).
  
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

  // Compute EMST using Kruskal's algorithm. This step takes O(n alpha(n)) time
  // in theory; for all practical purposes alpha(n) is constant, so linear time.
  std::map<int, int> comp_size;

  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    K::FT curr_edge_len = std::get<2>(*e);
    if (curr_edge_len > r) break;
    if (std::get<0>(*e) < k || std::get<1>(*e) < k) continue;
    
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    
    int s1, s2;
    if (comp_size.count(c1) == 0) {
      s1 = 1;
    } else {
      s1 = comp_size[c1];
    }
    
    if (comp_size.count(c2) == 0) {
      s2 = 1;
    } else {
      s2 = comp_size[c2];
    }
    
    if (c1 != c2) {
      uf.link(c1, c2);
      int c3 = uf.find_set(std::get<0>(*e));
      comp_size[c3] = s1 + s2;
      if (--n_components == 1) break;
    }
  }
  
  int best = 1;
  for (auto p : comp_size) {
    best = std::max(best, p.second);
  }
  
  return best;
}

void testcase() {
  int n; long r; std::cin>> n >> r;
  r *= r;
  
  std::vector<IPoint> points;
  for (Index i = 0; i < n; ++i) {
    int x, y;
    std::cin >> x >> y;
    points.emplace_back(K::Point_2(x, y), i);
  }
  
  int low = 1, high = n;
  int best = 1;
  while (low <= high) {
    int k = (low + high) / 2;
    int ans = std::min(k, solve(points, k, n, r));
    best = std::max(ans, best);

    if (ans >= k) {
      low = k + 1;
    } else {
      high = k - 1;
    }
  }
  
  std::cout << best << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}