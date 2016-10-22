// clang++ -std=c++11 *.cpp

#include "WUIPMNode.h"

using namespace std;

int main () {

  shared_ptr<WUIPMNode> root = make_shared<WUIPMNode>();
  shared_ptr<WUIPMNode> child = make_shared<WUIPMNode>();
  child->SetParent(root);
  root->AddChild(child);
  child->Print();
  root->Print();

  //child.reset();
  root.reset();
  child.reset();
  return 0;
}
