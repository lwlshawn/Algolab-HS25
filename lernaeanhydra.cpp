///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

typedef std::vector<int> VI;

int compute_overlap(VI& old_pattern, VI& curr_pattern, int k) {
  int last_old = old_pattern[old_pattern.size() - 1];

  for (int d = k - 1; d >= 0; d--) {
    bool overlap_possible = true;
    
    //check if overlap of length d is possible
    for (int t = 0; t < d; t++) {
      if (old_pattern[k - d + t] != curr_pattern[t]) overlap_possible = false;
      if (!overlap_possible) break;
    }
    if (!overlap_possible) continue;
    
    for (int t = d; t < k; t++) {
      if (curr_pattern[t] <= last_old) overlap_possible = false;
      if (!overlap_possible) break;
    }
    if (!overlap_possible) continue;
    return d;
  }
  
  return -1;
}

void testcase() {
  int n, m; std::cin >> n >> m;
  int k, d; std::cin >> k >> d;
  
  
  std::vector<std::vector<std::pair<int, VI>>> patterns(n);
  for (int i = 0; i < m; i++) {
    VI pattern;
    for (int j = 0; j < k; j++) {
      int h_i; std::cin >> h_i;
      pattern.push_back(h_i);
      
      if (j == k-1) {
        if (h_i == 0) patterns[0].emplace_back(k, pattern);
        else patterns[h_i].emplace_back(-1, pattern);
      }
    }
  }
  
  for (int i = 1; i < n; i++) {
    for (auto &p1 : patterns[i]) {
      VI curr_pattern = p1.second;

      for (auto &p2 : patterns[i-1]) {
        int old_score = p2.first;
        if (old_score == -1) continue;
        
        VI old_pattern = p2.second;
        int best_overlap = compute_overlap(old_pattern, curr_pattern, k);
        
        if (best_overlap == -1) continue;
        int new_score = old_score + k - best_overlap;
        
        if (p1.first == -1 || new_score < p1.first) {
          p1.first = new_score;
        }
      }
    }
  }
  
  int best_score = -1;
  for (auto &p : patterns[n-1]) {
    if (p.first == -1) continue;
    if (best_score == -1) best_score = p.first;
    else best_score = std::min(best_score, p.first);
  }
  if (best_score == -1) std::cout << "Impossible!" << std::endl;
  else std::cout << best_score << std::endl;
  
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}