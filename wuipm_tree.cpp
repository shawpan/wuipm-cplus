#include "wuipm_tree.h"
// Print subtree rooted at root
void WUIPMTree::PrintTree(std::shared_ptr<WUIPMNode> root, std::string prefix, bool details = false) const {
  root->Print(prefix, details);
  std::vector<std::shared_ptr<WUIPMNode> > children = root->children();
  for (size_t i = 0; i < children.size(); i++) {
    PrintTree(children[i], prefix + "    ", details);
  }
}

// Print the entire tree
void WUIPMTree::Print (bool details = false) const {
  std::cout << "Printing WUIPM Tree" << std::endl;
  std::cout << "------------------------" << std::endl;
  PrintTree(root_, "", details);
}

// Update values for the matched branch
void WUIPMTree::UpdateExistingBranch (
                  std::shared_ptr<WUIPMNode>& child,
                  std::shared_ptr<WUIPMNode>& current_root,
                  double probabilty_value,
                  double max_so_far,
                  double second_max_so_far
                ) {
  child->p_cap(child->p_cap() + max_so_far * probabilty_value);
  if( second_max_so_far > child->p_proxy() ) {
    child->p_proxy(second_max_so_far);
  }
  current_root = child;
}

// Add new child to the branch
void WUIPMTree::ProcessItemInNewBranch (
                  std::shared_ptr<WUIPMNode>& current_root,
                  int feature_id,
                  double probabilty_value,
                  double max_so_far,
                  double second_max_so_far
                ) {
  std::shared_ptr<WUIPMNode> new_child = std::make_shared<WUIPMNode>(
                                            feature_id, /* feature_id */
                                            max_so_far * probabilty_value, /* pCap*/
                                            second_max_so_far /* pProxy */
                                          );
  new_child->p_cap(probabilty_value * max_so_far);
  new_child->p_proxy(second_max_so_far);
  current_root->AddChild(new_child);
  new_child->parent(current_root);
  current_root = new_child;
}

// Update temporary max and second max values
void WUIPMTree::UpdateMaxValues (double& max_so_far, double& second_max_so_far, double probabilty_value) {
  if (probabilty_value > max_so_far) {
    second_max_so_far = max_so_far;
    max_so_far = probabilty_value;

    return;
  }
  if (probabilty_value > second_max_so_far) {
    second_max_so_far = probabilty_value;
  }

  return;
}

// Process an item of the row and return true if successful
bool WUIPMTree::ProcessItemInExistingBranch (
                   int feature_id,
                   std::shared_ptr<WUIPMNode>& current_root,
                   double max_so_far,
                   double second_max_so_far,
                   double probabilty_value
                 ) {
  std::vector<std::shared_ptr<WUIPMNode> > children = current_root->children();
  for (size_t j = 0; j < children.size(); j++) {
    // If branch is matched
    if (children[j]->feature_id() == feature_id) {
      UpdateExistingBranch(children[j], current_root, probabilty_value, max_so_far, second_max_so_far);
      return true;
    }
  }

  return false;
}

bool WUIPMTree::ItemCompare (const std::pair<double, std::pair<int, double> >& left, const std::pair<double, std::pair<int, double> >& right) {
  return left.first > right.first;
}

// Get row sorted and transformed according to expected support of items
std::vector<std::pair<double, std::pair<int, double> > > WUIPMTree::GetTransformedRow(std::vector<std::pair<int, double> >& row) {
  std::vector<std::pair<double, std::pair<int, double> > > transformed_row;
  // Add expected support of items for sorting
  for (auto it = row.begin(); it != row.end(); it++) {
    transformed_row.push_back(std::make_pair(expected_support_of_items_[it->first], std::make_pair(it->first, it->second)));
  }
  sort(transformed_row.begin(), transformed_row.end(), WUIPMTree::ItemCompare);

  return transformed_row;
}

// Insert a data tuple
void WUIPMTree::InsertRow (std::vector<std::pair<int, double> > row) {
  std::shared_ptr<WUIPMNode> current_root = root_;
  double max_so_far = 0, second_max_so_far = 0;
  bool is_same_branch = true;
    std::vector<std::pair<double, std::pair<int, double> > > transformed_row = GetTransformedRow(row);

  for (auto transformed_row_it = transformed_row.begin(); transformed_row_it != transformed_row.end(); transformed_row_it++) {
    if (transformed_row_it->first == 0.0) {
      break;
    }
    auto it = transformed_row_it->second;
    if (is_same_branch) {
      is_same_branch = ProcessItemInExistingBranch(it.first, current_root, max_so_far, second_max_so_far, it.second);
    }
    // If branch has diverted
    if (! is_same_branch) {
      ProcessItemInNewBranch(current_root, it.first, it.second, max_so_far, second_max_so_far);
    }
    // Update max and second max
    UpdateMaxValues(max_so_far, second_max_so_far, it.second);
  }
}

// Update expected support of items for row
void WUIPMTree::CalculateExpectedSupportOfItemsForRow(std::vector<std::pair<int, double> > row) {
  for (auto item_iterator = row.begin(); item_iterator != row.end(); ++item_iterator) {
    expected_support_of_items_[item_iterator->first] += item_iterator->second;
  }
}

// Update expected support of items for row
void WUIPMTree::RemoveItemsLessThanThreshold() {
  for (auto item_iterator = expected_support_of_items_.begin(); item_iterator != expected_support_of_items_.end(); ++item_iterator) {
    if (expected_support_of_items_[item_iterator->first] < minimum_support_threshold_) {
      expected_support_of_items_[item_iterator->first] = 0;
    }
  }
}

// Construc WUIPMTree from database
void WUIPMTree::Construct () {
  std::vector<std::vector<std::pair<int, double> > > udb = {
    //{ {4 , 0.6}, {1, 0.5}, {3, 0.4}, {2, 0.9}, {5, 0.3} },
    { {5 , 0.3}, {3, 0.4}, {1, 0.5}, {2, 0.9}, {4, 0.6} },
    { {4 , 0.5}, {1, 0.6}, {3, 0.4}, {2, 0.3} },
    { {4 , 0.2}, {1, 0.1}, {3, 0.9}, {5, 0.4} },
    { {4 , 0.2}, {1, 0.9}, {3, 0.1}, {5, 0.8} },
    { {4 , 0.9}, {1, 0.1}, {3, 0.2}, {2, 0.3} },
    { {4 , 0.5}, {1, 0.1}, {3, 0.2}, {2, 0.3} },
    { {4 , 0.4}, {2, 0.2}, {5, 0.3} }
  };
  for (auto row_it = udb.begin(); row_it != udb.end(); ++row_it) {
    CalculateExpectedSupportOfItemsForRow(*row_it);
  }
  for (auto row_it = udb.begin(); row_it != udb.end(); ++row_it) {
    InsertRow(*row_it);
  }
}
