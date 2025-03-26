///1
#include <iostream>
#include <vector>
#include <unordered_map>

void testcase() {
  int n, k, w; std::cin >> n >> k >> w;
  // std::cerr << "testcase begins\n";
  
  std::vector<int> islands;
  for (int i = 0; i < n; i++) {
    int c_i; std::cin >> c_i;
    islands.push_back(c_i);
  }
  
  std::vector<std::vector<int>> waterways;
  for (int i = 0; i < w; i++) {
    std::vector<int> waterway;
    int l_i; std::cin >> l_i;
    for (int j = 0; j < l_i; j++) {
      int r_j; std::cin >> r_j;
      waterway.push_back(r_j);
    }
    waterways.push_back(waterway);
  }
  
  int max_plan = 0;
  if (islands[0] == k) max_plan = 1; 
  
  //first we find solutions involving exactly one waterway
  for (int i = 0; i < w; i++) {
    std::vector<int> curr_waterway = waterways[i];
    int l = 0; int r = 0;
    long sum = islands[curr_waterway[0]];
    while (r < curr_waterway.size()) {
      if (sum < k) {
        r++;
        if (r < curr_waterway.size()) {
          sum += islands[curr_waterway[r]];
        }
      } else { //sum <= k
        sum -= islands[curr_waterway[l]];
        l++;
      }
      
      if (sum == k) {
        if (r - l + 1 > max_plan) max_plan = r - l + 1;
      }
    }
  }
  
  //then we find solutions involving two waterways.
  std::unordered_map<long, int> dct; //dct[value] = num islands summing to value
  long target = k - islands[0];
  if (target > 0) {
    for (int i = 0; i < w; i++) {
      std::vector<int> curr_waterway = waterways[i];
      long sum = 0;
      
      for (int j = 1; j < curr_waterway.size(); j++) {
        sum += islands[curr_waterway[j]];
        if (sum > k) break;
        if (dct.count(target - sum) != 0) { //some set of islands sum to (target - sum)
          if (dct[target - sum] + j + 1 > max_plan) max_plan = dct[target - sum] + j + 1;
        }
      }
      
      //only add the waterway's information to the dct after processing it to avoid counting against itself.
      sum = 0;
      for (int j = 1; j < curr_waterway.size(); j++) {
        sum += islands[curr_waterway[j]];
        if (sum > k) break;
        if (dct.count(sum) == 0) dct[sum] = j;
        else {
          if (dct[sum] < j) dct[sum] = j;
        }
      }
      
    }  
  }
  
  std::cout << max_plan << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}