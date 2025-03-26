#include <iostream>
#include <algorithm>
#include <vector>

void solve() {
  int n; std::cin >> n;
  std::vector<int> parasols(n, -1);
  
  for (int i = 0; i < n; i++) {
    int p; std::cin >> p;
    parasols[i] = p;
  }
  std::sort(parasols.begin(), parasols.end());
  
  std::vector<int> best_locs;
  int best_count, best_distance, l, r; best_distance = best_count = l = r = 0;
  while (r < n) {
    while (parasols[r] <= parasols[l] + 200 && r < n) {
      r++;
    }
    //we've reached a parasol such that parasols[r] > parasols[l] + 200 OR r == n
    //currently we've managed to cover all parasols from l up to r - 1. that's r - l many.
    //and the best distance is ceil((parasols[r-1] - parasols[l]) / 2)
    
    //COMPUTING OPTIMAL
    if (r - l > best_count) { //we've covered more parasols then the best
      best_count = r - l;
      
      best_locs.clear();
      if ((parasols[r-1] - parasols[l]) % 2 == 0) { //best distance even
        best_distance = (parasols[r-1] - parasols[l]) / 2;
        best_locs.push_back(parasols[l] + best_distance);
      } else { //best distance is odd
        best_distance = ((parasols[r-1] - parasols[l]) / 2) + 1;
        best_locs.push_back(parasols[l] + best_distance - 1);
        best_locs.push_back(parasols[l] + best_distance);
      }
      
    } else if (r - l == best_count) { //covered same # of parasols as current best 
      int candidate_distance = (parasols[r-1] - parasols[l]) % 2 == 0 ? (parasols[r-1] - parasols[l]) / 2 : ((parasols[r-1] - parasols[l]) / 2) + 1;
      if (candidate_distance < best_distance) {
        best_distance = candidate_distance;
        
        best_locs.clear();
        if ((parasols[r-1] - parasols[l]) % 2 == 0) { //best distance even
          best_distance = (parasols[r-1] - parasols[l]) / 2;
          best_locs.push_back(parasols[l] + best_distance);
        } else { //best distance is odd
          best_distance = ((parasols[r-1] - parasols[l]) / 2) + 1;
          best_locs.push_back(parasols[l] + best_distance - 1);
          best_locs.push_back(parasols[l] + best_distance);
        }
        
      } else if (candidate_distance == best_distance) {
        if ((parasols[r-1] - parasols[l]) % 2 == 0) {
          best_locs.push_back(parasols[l] + best_distance); 
        } else {
          best_locs.push_back(parasols[l] + best_distance - 1);
          best_locs.push_back(parasols[l] + best_distance);
        }
      }
    }
    
    // after the above, we've computed a new optimal if it exists.
    while (parasols[l] < parasols[r] - 200 && r < n) {
      l++;
    }
  }
  
  //output solution
  std::cout << best_count << ' ' << best_distance << std::endl;
  for (int loc : best_locs) {
    std::cout << loc << ' ';
  }
  std::cout << std::endl;
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}