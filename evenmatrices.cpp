#include <iostream>
#include <vector>

int nChooseTwo(int n) {
  return (n * (n - 1)) / 2;
}

void solve() {
  int n; std::cin >> n;
  std::vector<std::vector<int>> matrix(n, std::vector<int>(n, -1));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int xij; std::cin >> xij;
      matrix[i][j] = xij;
    }
  }
  
  // stores prefix sums of the columns.
  // so m[i][j] stores the sum of all elements from m[0][j] to m[i][j]
  std::vector<std::vector<int>> colSums(n, std::vector<int>(n, -1));
  for (int j = 0; j < n; j++) {
    for (int i = 0; i < n; i++) {
      if (i == 0) colSums[i][j] = matrix[i][j];
      else colSums[i][j] = matrix[i][j] + colSums[i - 1][j];
    }
  }
  
  long count = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      int even_parities, odd_parities, current_parity;
      even_parities = odd_parities = current_parity = 0;
      
      for (int k = 0; k < n; k++) {
        // generally we want the sum of element in the column from 0 up to j
        // minus the elements in the column from 0 up to i - 1.
        if (i == 0) current_parity = (current_parity + colSums[j][k]) % 2;
        else current_parity = (current_parity + colSums[j][k] - colSums[i - 1][k]) % 2;
        
        if (current_parity == 0) even_parities += 1;
        else odd_parities += 1;
      }
      
      count += even_parities + nChooseTwo(even_parities) + nChooseTwo(odd_parities);
    }
  }

  std::cout << count << std::endl;
}


// idea is to build on the parities idea for even pairs. I think O(n^3) unavoidable.
int main() {
  int t; std::cin >> t;
  for (int i = 0; i < t; i++) {
    solve();
  }
}