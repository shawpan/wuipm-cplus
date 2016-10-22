// clang++ -std=c++11 *.cpp

#include "wuipm_tree.h"

using namespace std;

int main () {

  shared_ptr<WUIPMTree> wuipm_tree = make_shared<WUIPMTree>();
  wuipm_tree->Print();

  return 0;
}
