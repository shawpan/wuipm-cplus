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

  shared_ptr<WUIPMTree> wuipm_tree = make_shared<WUIPMTree>();
  wuipm_tree->Construct(0.18 /* minimum support threshold 18% */, udb);
  //wuipm_tree->Print(false /* details off */);
  std::vector<std::vector<int> > interesting_patterns = wuipm_tree->GetInterestingPatterns();

  for (size_t i = 0; i < interesting_patterns.size(); i++) {
    std::cout << i + 1 << ") ";
    PrintPattern(interesting_patterns[i]);
  }

  return 0;
}
