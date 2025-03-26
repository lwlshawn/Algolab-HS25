///1
#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>

void testcase() {
  int n; std::cin >> n;
  std::vector<std::pair<int, int>> boats;
  for (int i = 0; i < n; i++) {
    int l, p; std::cin >> l >> p;
    boats.emplace_back(p, l); //(ring, boatlength)
  }
  std::sort(boats.begin(), boats.end());
  
  int num_boats = 1;
  int prev_boat = boats[0].first; //right endpoint of previous boat. Starts at ring_pos
  int curr_boat; //right endpoint of current boat
  for (int i = 1; i < boats.size(); i++) {
    if (i == 1) {
      //can always fit the first two boats
      curr_boat = std::max(prev_boat + boats[i].second, boats[i].first);
      num_boats++;
    } else {
      std::pair<int, int> boat = boats[i];
      int ring_pos = boat.first;
      int boat_len = boat.second;
      //if current ring position does not exceed the right endpoint of previous boat
      //check if improvement possible. Simply swapping out a boat; dont increment count
      if (ring_pos < curr_boat) {
        //new potential endpoint
        int new_end = std::max(ring_pos, prev_boat + boat_len);
        if (new_end < curr_boat) curr_boat = new_end;
      } else {
        //ring_pos >= curr_boat, can always fit a new boat
        prev_boat = curr_boat;
        curr_boat = std::max(curr_boat + boat_len, ring_pos);
        num_boats++;
        
      }
    }
  }
  std::cout << num_boats << std::endl;
}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}