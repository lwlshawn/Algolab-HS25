///2
#include <iostream>
#include <vector>
#include <algorithm>
#include <utility>
#include <cassert>
#include <tuple>

void testcase(int tnum) {
  int n, m; std::cin >> n >> m;
  std::vector<int> strengths;
  for (int i = 0; i < n; i++) {
    int s_i; std::cin >> s_i;
    strengths.push_back(s_i);
  }
  
  std::vector<int> weights;
  for (int i = 0; i < m; i++) {
    int w_i; std::cin >> w_i;
    weights.push_back(w_i);
  }
  
  std::sort(strengths.begin(), strengths.end()); //increasing
  std::sort(weights.begin(), weights.end());
  if (weights[weights.size() - 1] > strengths[strengths.size() - 1]) {
    std::cout << "impossible" << std::endl;
    return;
  }
  //one friend lifts one box at a time; strength denotes heaviest box they can lift
  //idea: everyone always just takes the heaviest remaining box they can lift
  //split the boxes into weight classes, with workers that can deal with them and iterate
  //i.e. class[i] = (boxes, workers), and just loop until all boxes dealt with

  std::vector<std::pair<int, int>> classes;
  int w_ind = 0;
  int s_ind = 0;
  
  while (s_ind < strengths.size()) {
    int num_workers = 1;
    s_ind++;
    
    while (s_ind < strengths.size() && strengths[s_ind] == strengths[s_ind - 1]) {
      num_workers++;
      s_ind++;
    }
    
    int num_boxes = 0;
    while (w_ind < weights.size() && weights[w_ind] <= strengths[s_ind-1]) {
      num_boxes++;
      w_ind++;
    }
    classes.emplace_back(num_boxes, num_workers);
  }

  /*
    For each class: 
      if more workers then boxes remaining, shift extra workers down
      Otherwise, if boxes <= num workers, process boxes
    
    end of iteration: check if any boxes left. If no, submit time + 2
    if yes, take time + 3 and continue;
  */
  
  int t = 0;
  bool finished = false;
  while (!finished) {
    t += 2;
    for (int i = classes.size() - 1; i > -1; i--) {
      int boxes, workers;
      std::tie(boxes, workers) = classes[i]; 
      if (workers > boxes && i > 0) { //shift excess workers down
        int excess_workers = workers - boxes;
        classes[i-1].second += excess_workers;
        classes[i].second -= excess_workers;
      }
      
      //letting this run unchecked led to weird overflow errors!!
      if (classes[i].first > 0) classes[i].first -= classes[i].second;
      // if (tnum == 10) std::cerr << classes[i].first << ' ';
    }
    
    finished = true;
    for (int i = classes.size() - 1; i > -1; i--) {
      if (classes[i].first <= 0 && i > 0) { //if class i has no boxes remaining
        //if no boxes remaining shift the workers down to the next class
        classes[i-1].second += classes[i].second; //add workers to class i-1
        classes[i].second = 0;
      }
      
      if (classes[i].first > 0) {
        finished = false;
      }
    }

    if (!finished) t++;
  }
  std::cout << t << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase(i);
  }
}