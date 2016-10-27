#ifndef WUIPM_CPLUS_WUIPM_TREE_H_
#define WUIPM_CPLUS_WUIPM_TREE_H_

#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <algorithm>
#include "wuipm_node.h"

#define PAIR_INT_DOUBLE std::pair<int, double>
#define PAIR_DOUBLE_INT_DOUBLE std::pair<double, PAIR_INT_DOUBLE>

class WUIPMTree {
  public:
    WUIPMTree() {
      root_ = std::make_shared<WUIPMNode>();
    };

    ~WUIPMTree() {};

    void Print(bool) const;
    void Construct(double, std::vector<std::vector<PAIR_INT_DOUBLE> >&);
    std::vector<std::vector<int> > GetInterestingPatterns();

    std::shared_ptr<WUIPMNode> root() const { return root_; };

    static bool ItemCompare (const PAIR_DOUBLE_INT_DOUBLE& left, const PAIR_DOUBLE_INT_DOUBLE& right);

  private:
    void PrintTree(std::shared_ptr<WUIPMNode>, std::string, bool) const;
    void InsertRow(std::vector<PAIR_INT_DOUBLE>);
    void UpdateExistingBranch(std::shared_ptr<WUIPMNode>&, std::shared_ptr<WUIPMNode>&, double, double, double);
    void ProcessItemInNewBranch(std::shared_ptr<WUIPMNode>&, int, double, double, double);
    void UpdateMaxValues(double&, double&, double);
    bool ProcessItemInExistingBranch(int, std::shared_ptr<WUIPMNode>&, double, double, double);
    void CalculateExpectedSupportOfItemsForRow(std::vector<PAIR_INT_DOUBLE>);
    void ResetItemsLessThanThreshold();
    void ProjectTree(int);
    void ResetTemporaryValues(int, std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > >);
    std::vector<PAIR_DOUBLE_INT_DOUBLE> GetTransformedRow (std::vector<PAIR_INT_DOUBLE>&);

    std::shared_ptr<WUIPMNode> root_;
    std::unordered_map<int, double> expected_support_of_items_;
    // Keep node adresses in tree grouped by feature id
    std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > feature_to_node_in_tree_;
    double minimum_support_threshold_;
    double minimum_support_threshold_percentage_;
};

#endif // WUIPM_CPLUS_WUIPM_TREE_H_
