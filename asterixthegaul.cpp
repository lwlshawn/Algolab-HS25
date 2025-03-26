///3
#include <iostream>
#include <vector>
#include <utility>
#include <tuple>
#include <algorithm>

typedef std::pair<long, long> P;
typedef std::tuple<long, long, long> S;

void construct_sets(std::vector<P>& moves, std::vector<S>& move_sets, long D, long T) {
  int n = moves.size();
  for (int i = 0; i < (1<<n); i++) {
    long dist_sum = 0;
    long time_sum = 0;
    int count_moves = 0;
    
    for (int j = 0; j < n; j++) {
      if ((1<<j & i) == 1<<j) {
        dist_sum += moves[j].first;
        time_sum += moves[j].second;
        count_moves++;
      }
    }
    
    if (time_sum < T) move_sets.emplace_back(time_sum, dist_sum, count_moves);
  }
}

void testcase() {
  // std::cerr << "testcase begins" << std::endl;
  int n, m; long D, T; std::cin >> n >> m >> D >> T;
  std::vector<P> moves1, moves2;
  for (int i = 0; i < n/2; i++) {
    long d, t; std::cin >> d >> t;
    moves1.emplace_back(d, t);
  }
  
  for (int i = n/2; i < n; i++) {
    long d, t; std::cin >> d >> t;
    moves2.emplace_back(d, t);
  }
  
  std::vector<long> potion(m+1, 0);
  for (int i = 0; i < m; i++) {
    long s_i; std::cin >> s_i;
    potion[i+1] = s_i;
  }
  
  std::vector<S> move_sets1, move_sets2;
  construct_sets(moves1, move_sets1, D, T);
  construct_sets(moves2, move_sets2, D, T);

  int low = 0; int high = m;
  bool captured = true;
  int best = -1;
  while (low <= high) {
    captured = true;
    int mid = (high + low) / 2;
    
    std::vector<P> list2;
    for (S set : move_sets2) {
      long t = std::get<0>(set);
      long d = std::get<1>(set);
      long m = std::get<2>(set);
      
      long new_dist = d + (potion[mid] * m);
      list2.emplace_back(new_dist, t);
    }
    std::sort(list2.begin(), list2.end());
    
    std::vector<long> suffix_min(list2.size());
    suffix_min[list2.size() - 1] = list2[list2.size() - 1].second;
    for (int i = list2.size() - 2; i >= 0; i--) {
      suffix_min[i] = std::min(suffix_min[i+1], list2[i].second);
    }
    
    for (S s1 : move_sets1) {
      long t_1 = std::get<0>(s1);
      long d_1 = std::get<1>(s1);
      long m_1 = std::get<2>(s1);
      d_1 += m_1*potion[mid];
      
      int low2 = 0; int high2 = list2.size() - 1;
      while (low2 <= high2) {
        int mid2 = (low2 + high2) / 2;
        long d_2 = list2[mid2].first;
        
        if (d_1 + d_2 < D) {
          low2 = mid2 + 1;
        } else {
          high2 = mid2 - 1;
          if (t_1 + suffix_min[mid2] < T) {
            captured = false;
            break;
          }
        }
      }
      if (!captured) break;
    }
    
    if (!captured) {
      high = mid - 1;
      if (best == -1 || mid < best) best = mid;
    } else {
      low = mid + 1;
    }
  }
  
  if (best == -1) {
    std::cout << "Panoramix captured" << std::endl;
  } else {
    std::cout << best << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}