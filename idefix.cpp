///3
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

std::ostream& operator<<(std::ostream& o, const Edge& e) {
  return o << std::get<0>(e) << " " << std::get<1>(e) << " " << std::get<2>(e);
}

int num_bones(EdgeV& edges, K::FT cutoff, int n, int m, std::map<K::Point_2, K::Point_2>& nearest_tree, std::vector<K::Point_2>& bones, std::map<K::Point_2, Index>& tree_indices) {
  // setup and initialize union-find data structure
  boost::disjoint_sets_with_storage<> uf(n);
  Index n_components = n;
  // ... and process edges in order of increasing length
  for (EdgeV::const_iterator e = edges.begin(); e != edges.end(); ++e) {
    // determine components of endpoints
    Index c1 = uf.find_set(std::get<0>(*e));
    Index c2 = uf.find_set(std::get<1>(*e));
    K::FT curr_len = std::get<2>(*e);
    if (curr_len > cutoff) break;
    
    if (c1 != c2) {
      // this edge connects two different components => part of the emst
      uf.link(c1, c2);
      // std::cout << *e << "\n";
      if (--n_components == 1) break;
    }
  }
  
  std::map<int, int> counts;
  for (int i = 0; i < m; i++) {
    K::Point_2 curr_bone = bones[i];
    K::Point_2 nearest = nearest_tree[curr_bone];
    if (4 * CGAL::squared_distance(curr_bone, nearest) <= cutoff) {
      int c_ind = uf.find_set(tree_indices[nearest]);
      if (counts.count(c_ind) == 0) {
        counts[c_ind] = 1;
      } else {
        counts[c_ind]++;
      }
    }
  }
  
  int max_bones = 0;
  for (auto p : counts) {
    max_bones = std::max(max_bones, p.second);
  }
  
  return max_bones;
}


void testcase() {
  int n, m, k; K::FT s; std::cin >> n >> m >> s >> k;

  typedef std::pair<K::Point_2,Index> IPoint;
  std::vector<IPoint> points;
  std::map<K::Point_2, Index> tree_indices;
  for (Index i = 0; i < n; ++i) {
    int x, y; std::cin >> x >> y;
    K::Point_2 tree(x, y);
    
    points.emplace_back(tree, i);
    tree_indices[tree] = i;
  }
  
  Delaunay t;
  t.insert(points.begin(), points.end());
  
  
  std::vector<K::Point_2> bones;
  std::map<K::Point_2, K::Point_2> nearest_tree;
  std::set<K::FT> lengths_set;

  for (int i = 0; i < m; i++) {
    int x, y; std::cin >> x >> y;
    K::Point_2 bone(x, y);
    
    bones.emplace_back(bone);
    auto vh = t.nearest_vertex(bone);
    nearest_tree[bone] = vh->point();
    
    lengths_set.insert(4*CGAL::squared_distance(bone, vh->point()));
  }
  
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
    lengths_set.insert(t.segment(e).squared_length());
  }
  std::sort(edges.begin(), edges.end(),
      [](const Edge& e1, const Edge& e2) -> bool {
        return std::get<2>(e1) < std::get<2>(e2);
            });

  std::vector<K::FT> lengths(lengths_set.begin(), lengths_set.end());
  
  int res = num_bones(edges, s, n, m, nearest_tree, bones, tree_indices);

  int low = 0; int high = lengths.size() - 1;
  K::FT best_val = -1;
  while (low <= high) {
    int mid = (low + high) / 2;
    K::FT curr_cutoff = lengths[mid];
    int count = num_bones(edges, curr_cutoff, n, m, nearest_tree, bones, tree_indices);
    if (count >= k) {
      if (best_val == -1 || curr_cutoff < best_val) {
        best_val = curr_cutoff;
      }
      high = mid-1; //recurse to lower lengths
    } else {
      low = mid+1;
    }
  }

  std::cout << std::fixed << std::setprecision(0);
  std::cout << res << ' ' << best_val << std::endl;

}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}
