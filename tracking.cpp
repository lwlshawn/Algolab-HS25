#include <iostream>
#include <tuple>
#include <vector>
#include <queue>

typedef std::tuple<int, int, int> triple;

void testcase() {
  int n, m, k, x, y; std::cin >> n >> m >> k >> x >> y;
  std::vector<std::vector<triple>> adj_list(n);
  for (int i = 0; i < m; i++) {
    int a, b, c, d; std::cin >> a >> b >> c >> d;
    adj_list[a].emplace_back(b, c, d);
    adj_list[b].emplace_back(a, c, d);
  }
  
  std::priority_queue<triple, std::vector<triple>, std::greater<triple>> pq;
  std::vector<std::vector<int>> dist(n, std::vector<int>(k+1, -1));
  //usually the priority queue has (distance, vertex)
  //now we want (distance, vertex, numrivers)
  dist[x][0] = 0;
  pq.push(triple(0, x, 0));
  while (!pq.empty()) {
    triple curr = pq.top();
    pq.pop();
    long curr_dist = std::get<0>(curr);
    int curr_vert = std::get<1>(curr);
    int num_rivers = std::get<2>(curr);
    
    if (curr_dist > dist[curr_vert][num_rivers]) {
      //curr_vert placed into pq implies its dist no longer -1
      //this represents an outdated entry
      continue;
    }

    for (triple t : adj_list[curr_vert]) {
      int b = std::get<0>(t);
      int c = std::get<1>(t);
      int d = std::get<2>(t);

      if (d == 0 || num_rivers == k) {
        if (dist[b][num_rivers] == -1 || dist[b][num_rivers] > dist[curr_vert][num_rivers] + c) {
          dist[b][num_rivers] = dist[curr_vert][num_rivers] + c;
          pq.push(triple(dist[b][num_rivers], b, num_rivers));
        }
      } else { //d==1 && num_rivers < k
        if (dist[b][num_rivers+1] == -1 || dist[b][num_rivers+1] > dist[curr_vert][num_rivers] + c) {
          dist[b][num_rivers+1] = dist[curr_vert][num_rivers] + c;
          pq.push(triple(dist[b][num_rivers+1], b, num_rivers+1));
        }
      }
    }
  }
  std::cout << dist[y][k] << std::endl;

}

int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    testcase();
  }
}