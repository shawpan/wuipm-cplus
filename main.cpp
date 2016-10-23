// clang++ -std=c++11 *.cpp

#include "wuipm_tree.h"

using namespace std;

int main () {

  shared_ptr<WUIPMTree> wuipm_tree = make_shared<WUIPMTree>();
  wuipm_tree->Construct();
  wuipm_tree->Print(false /* details off */);

  return 0;
}
