#include "wuipm_tree.h"

void WUIPMTree::PrintTree(std::shared_ptr<WUIPMNode> root) const {
  root->Print();
  std::vector<std::shared_ptr<WUIPMNode> > children;
  for (size_t i = 0; i < children.size(); i++) {
    PrintTree(children[i]);
  }
}

void WUIPMTree::Print () const {
  std::cout << "********************" << std::endl;
  std::cout << "Printing WUIPM Tree" << std::endl;
  PrintTree(root_);
  std::cout << "********************" << std::endl;
}
