#ifndef WUIPM_CPLUS_WUIPM_TREE_H_
#define WUIPM_CPLUS_WUIPM_TREE_H_

#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include "wuipm_node.h"

class WUIPMTree {
  public:
    WUIPMTree() { root_ = std::make_shared<WUIPMNode>(); };
    ~WUIPMTree() {};

    void Print(bool) const;

    std::shared_ptr<WUIPMNode> root() const { return root_; };
    void Construct();

  private:
    void PrintTree(std::shared_ptr<WUIPMNode>, std::string, bool) const;
    void InsertRow(std::unordered_map<int, double>);
    std::shared_ptr<WUIPMNode> root_;

};

#endif // WUIPM_CPLUS_WUIPM_TREE_H_
