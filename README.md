# Mining Interesting Patterns from Uncertain Databases
Published Journal is here [WUIPM](http://www.sciencedirect.com/science/article/pii/S0020025516301487)

# Note
Requires C++11
`clang++ -std=c++11 *.cpp`

# Sample usage
```cpp
// clang++ -std=c++11 *.cpp

#include "wuipm_tree.h"

using namespace std;

void PrintPattern (std::vector<int> pattern) {
  for (size_t i = 0; i < pattern.size(); i++) {
    std::cout << pattern[i] << " ";
  }
  std::cout << std::endl;
}

int main () {

  std::vector<std::vector<PAIR_INT_DOUBLE> > udb = {
    //{ {4 , 0.6}, {1, 0.5}, {3, 0.4}, {2, 0.9}, {5, 0.3} },
    { {5 , 0.3}, {3, 0.4}, {1, 0.5}, {2, 0.9}, {4, 0.6} },
    { {4 , 0.5}, {1, 0.6}, {3, 0.4}, {2, 0.3} },
    { {4 , 0.2}, {1, 0.1}, {3, 0.9}, {5, 0.4} },
    { {4 , 0.2}, {1, 0.9}, {3, 0.1}, {5, 0.8} },
    { {4 , 0.9}, {1, 0.1}, {3, 0.2}, {2, 0.3} },
    { {4 , 0.5}, {1, 0.1}, {3, 0.2}, {2, 0.3} },
    { {4 , 0.4}, {2, 0.2}, {5, 0.3} }
  };
  std::unordered_map<int, double> weight = { {4, 0.1}, {1, 0.2}, {3, 0.3}, {2, 0.4}, {5, 0.5} };

  shared_ptr<WUIPMTree> wuipm_tree = make_shared<WUIPMTree>();
  wuipm_tree->Construct(0.18 /* minimum support threshold 18% */, 0.5 /* minimum affinity 50% */, 0.4 /* minimum weighted affinity 40% */,  udb, weight);
  //wuipm_tree->Print(false /* details off */);
  std::vector<std::vector<int> > interesting_patterns = wuipm_tree->GetInterestingPatterns();

  for (size_t i = 0; i < interesting_patterns.size(); i++) {
    std::cout << i + 1 << ") ";
    PrintPattern(interesting_patterns[i]);
  }

  return 0;
}

```
# Sample Output
```
minExpectedSupport = 18%, minUConf = 0.0, minWUConf = 0.0
1) 5
2) 5 4
3) 5 1
4) 5 3
5) 2
6) 3
7) 1
8) 4

minExpectedSupport = 18%, minUConf = 50%, minWUConf = 0.0
1) 5
2) 5 1
3) 5 3
4) 2
5) 3
6) 1
7) 4

minExpectedSupport = 18%, minUConf = 50%, minWUConf = 40%
1) 5
2) 5 3
3) 2
4) 3
5) 1
6) 4

```

# TODO
* Introduce database adapter so that different databases can be used without loading into memory.
* Refactor and clean coding styles
