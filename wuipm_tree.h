#ifndef WUIPM_CPLUS_WUIPM_TREE_H_
#define WUIPM_CPLUS_WUIPM_TREE_H_

#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include "wuipm_node.h"

class WUIPMTree {
  public:
    WUIPMTree(double minimum_support_threshold) {
      root_ = std::make_shared<WUIPMNode>();
      minimum_support_threshold_ = minimum_support_threshold;
    };
    ~WUIPMTree() {};

    void Print(bool) const;
    void Construct();

    std::shared_ptr<WUIPMNode> root() const { return root_; };


    static bool ItemCompare (const std::pair<double, std::pair<int, double> >& left, const std::pair<double, std::pair<int, double> >& right);

  private:
    void PrintTree(std::shared_ptr<WUIPMNode>, std::string, bool) const;
    void InsertRow(std::vector<std::pair<int, double> >);
    void UpdateExistingBranch(std::shared_ptr<WUIPMNode>&, std::shared_ptr<WUIPMNode>&, double, double, double);
    void ProcessItemInNewBranch(std::shared_ptr<WUIPMNode>&, int, double, double, double);
    void UpdateMaxValues(double&, double&, double);
    bool ProcessItemInExistingBranch(int, std::shared_ptr<WUIPMNode>&, double, double, double);
    void CalculateExpectedSupportOfItemsForRow(std::vector<std::pair<int, double> >);
    void RemoveItemsLessThanThreshold();
    std::vector<std::pair<double, std::pair<int, double> > > GetTransformedRow (std::vector<std::pair<int, double> >&);

    std::shared_ptr<WUIPMNode> root_;
    std::unordered_map<int, double> expected_support_of_items_;
    double minimum_support_threshold_;

};

#endif // WUIPM_CPLUS_WUIPM_TREE_H_
