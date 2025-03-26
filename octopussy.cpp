#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>

//guarantees that children have smaller time then their parent
//so solving the bombs in sorted order now guarantees a parent is solved after the child
//optimality is also simple to see: in the sorted order, even if we could change two bombs
//if the original order didn't work, certainly the new order can't be an improvement.
//because lets say you swap the bomb that explodes to try and improve it
//if you move it up, it explodes later, if you move it down, whatever you swapped it
//with from below should explode in its spot anyway, since its time is <=

void update_times(int root, int n, std::vector<int>& times) {
  if (root >= (n-1) / 2) return;
  times[root*2 + 1] = std::min(times[root*2 + 1], times[root] - 1);
  times[root*2 + 2] = std::min(times[root*2 + 2], times[root] - 1);
  update_times(root*2 + 1, n, times);
  update_times(root*2 + 2, n, times);
}

void testcase() {
  int n; std::cin >> n;
  std::vector<int> times;
  for (int i = 0; i < n; i++) {
    int t_i; std::cin >> t_i;
    times.push_back(t_i);
  }

  update_times(0, n, times);

  std::sort(times.begin(), times.end());
  for (int i = 0; i < times.size(); i++) {
    if (times[i] < i+1) {
      std::cout << "no" << std::endl;
      return;
    }
  }
  std::cout << "yes" << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}