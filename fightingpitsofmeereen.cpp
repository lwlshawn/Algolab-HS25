///3
#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <set>
#include <cmath>

typedef std::vector<int> VI;
typedef std::vector<std::map<int, long>> VM;
typedef std::vector<VM> VVM;
typedef std::tuple<int,int,int,int> State;

long score(int d, int f1, int f2, int f3, int m, int k) {
  std::set<int> distinct;
  if (f2 != k) distinct.insert(f2);
  if (f3 != k) distinct.insert(f3);
  if (m == 3 && f1 != k) distinct.insert(f1);
  return 1000 * distinct.size() - pow(2, d);
}

std::tuple<int,int,int,int> int_to_state(int state) {
  int curr_state = state;
  int s2 = curr_state % 10;
  curr_state = (curr_state - s2) / 10;
  
  int s1 = curr_state % 10;
  curr_state = (curr_state - s1) / 10;
  
  int n2 = curr_state % 10;
  curr_state = (curr_state - n2) / 10;
  
  int n1 = curr_state % 10;
  curr_state = (curr_state - n1) / 10;
  
  return std::tuple<int,int,int,int>(n1, n2, s1, s2);
}

int state_to_int(int f1, int f2, int f3, int f4) {
  return f1 * 1000 + f2 * 100 + f3 * 10 + f4;
}

void testcase() {
  int n, k, m; std::cin >> n >> k >> m;
  VI fighters;
  for (int i = 0; i < n; i++) {
    int x; std::cin >> x;
    fighters.push_back(x);
  }
  
  //used an 4 digit int to store the fighters state, each int is a fighter n1 n2 s1 s2
  VM DP(n+1); 
  DP[1][state_to_int(k, fighters[0], k, k)] = 998;
  for (int i = 1; i < n; i++) {
    VM newDP(n+1);
    
    for (int d = 0; d <= n; d++) {
      if (DP[d].size() != 0) {
        for (auto p : DP[d]) { 
          //we use this to build a new state for DP[i][d+1] and DP[i][d-1]
          int old_state_int = p.first;
          long curr_score = p.second;
          
          int n1, n2, s1, s2;
          std::tie(n1, n2, s1, s2) = int_to_state(old_state_int);
          
          //based on the fighter going to either the north queue or the south queue
          //wlog lets always assume the north queue is the longer queue
          //if we add the fighter to the north, distance increases by 1
          //n2 shifts down to n1, and the current fighter becomes n2
          if (score(d+1, n1, n2, fighters[i], m, k) >= 0) {
            long new_north_score = curr_score + score(d+1, n1, n2, fighters[i], m, k);
            int new_n1 = n2;
            int new_n2 = fighters[i];
            int state_int = state_to_int(new_n1, new_n2, s1, s2);
            
            if (newDP[d+1].count(state_int) == 0) newDP[d+1][state_int] = new_north_score;
            else newDP[d+1][state_int] = std::max(new_north_score, newDP[d+1][state_int]);
          }
          
          if (d == 0) {
            //this is the special case; here the south queue "becomes the north queue"
            //since its now the longer queue.
            if (score(d+1, s1, s2, fighters[i], m, k) >= 0) {
              long new_south_score = curr_score + score(d+1, s1, s2, fighters[i], m, k);
              int new_s1 = s2;
              int new_s2 = fighters[i];
              int state_int = state_to_int(new_s1, new_s2, n1, n2);
              
              if (newDP[d+1].count(state_int) == 0) newDP[d+1][state_int] = new_south_score;
              else newDP[d+1][state_int] = std::max(new_south_score, newDP[d+1][state_int]);
            }
            
          } else {
            if (score(d-1, s1, s2, fighters[i], m, k) >= 0) {
              long new_south_score = curr_score + score(d-1, s1, s2, fighters[i], m, k);
              int new_s1 = s2;
              int new_s2 = fighters[i];
              int state_int = state_to_int(n1, n2, new_s1, new_s2);
              
              if (newDP[d-1].count(state_int) == 0) newDP[d-1][state_int] = new_south_score;
              else newDP[d-1][state_int] = std::max(new_south_score, newDP[d-1][state_int]);
            }
          }
        }
      }
    }
    DP = newDP;
  }
  
  //algorithm runs in something like n^2 * k^4 ~ 25 * 10^6 * 256 ~ 10^8 in the worst case
  //probably a severe estimate since k^4 represents a fully saturated state space which doesn't
  //really happen for every d i think.
  long best = -1;
  for (int d = 0; d <= n; d++) {
    for (auto p : DP[d]) {
      best = std::max(best, p.second);
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