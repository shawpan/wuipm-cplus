#ifndef WUIPM_CPLUS_WUIPM_TREE_H_
#define WUIPM_CPLUS_WUIPM_TREE_H_

#include <vector>
#include <iostream>
#include <memory>
#include "wuipm_node.h"

class WUIPMTree {
  public:
    WUIPMTree() { root_ = std::make_shared<WUIPMNode>(); };
    ~WUIPMTree() {};

    void Print();

    std::shared_ptr<WUIPMNode> GetRoot() const { return root_; };

  private:
    void PrintTree(std::shared_ptr<WUIPMNode>) const;
    std::shared_ptr<WUIPMNode> root_;
};

#endif // WUIPM_CPLUS_WUIPM_TREE_H_
