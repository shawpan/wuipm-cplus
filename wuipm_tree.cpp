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
                                            max_so_far * probabilty_value, /* pCap */
                                            second_max_so_far /* pProxy */
                                          );
  // Insert node address of this feature id
  feature_to_node_in_tree_[feature_id].push_back(new_child);

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

bool WUIPMTree::ItemCompare (const PAIR_DOUBLE_INT_DOUBLE& left, const PAIR_DOUBLE_INT_DOUBLE& right) {
  return left.first > right.first;
}

// Get row sorted and transformed according to expected support of items, remove items less than minimum support count
std::vector<PAIR_DOUBLE_INT_DOUBLE> WUIPMTree::GetTransformedRow(std::vector<PAIR_INT_DOUBLE>& row) {
  std::vector<PAIR_DOUBLE_INT_DOUBLE> transformed_row;
  // Add expected support of items for sorting
  for (auto it = row.begin(); it != row.end(); it++) {
    // If does not meet minimum support threshold, skip
    if (expected_support_of_items_[it->first] < minimum_support_threshold_) {
      continue;
    }
    transformed_row.push_back(std::make_pair(expected_support_of_items_[it->first], std::make_pair(it->first, it->second)));
  }
  sort(transformed_row.begin(), transformed_row.end(), WUIPMTree::ItemCompare);

  return transformed_row;
}

// Insert a data tuple
void WUIPMTree::InsertRow (std::vector<PAIR_INT_DOUBLE> row) {
  std::shared_ptr<WUIPMNode> current_root = root_;
  double max_so_far = 0, second_max_so_far = 0;
  bool is_same_branch = true;
    std::vector<PAIR_DOUBLE_INT_DOUBLE> transformed_row = GetTransformedRow(row);

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
void WUIPMTree::CalculateExpectedSupportOfItemsForRow(std::vector<PAIR_INT_DOUBLE> row) {
  for (auto item_iterator = row.begin(); item_iterator != row.end(); ++item_iterator) {
    expected_support_of_items_[item_iterator->first] += item_iterator->second;
  }
}

// Update expected support of items for row
void WUIPMTree::ResetItemsLessThanThreshold() {
  for (auto item_iterator = expected_support_of_items_.begin(); item_iterator != expected_support_of_items_.end(); ++item_iterator) {
    if (expected_support_of_items_[item_iterator->first] < minimum_support_threshold_) {
      item_iterator = expected_support_of_items_.erase(item_iterator);
    }
  }
}

// Reset temporary values to 0
void WUIPMTree::ResetTemporaryValues (int feature_id, std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > node_map) {
  for (auto node_iterator = feature_to_node_in_tree_[feature_id].begin(); node_iterator != feature_to_node_in_tree_[feature_id].end() ; ++node_iterator) {
    std::shared_ptr<WUIPMNode> node = *node_iterator;
    while (node->parent()) {
      node->temporary_p_cap(0.0);
      node->temporary_p_proxy(0.0);
      node = node->parent();
    }
  }
}

// Project a subtree given the feature
void WUIPMTree::ProjectTree (int feature_id) {
  ResetTemporaryValues(feature_id, feature_to_node_in_tree_);
  // For each node of this feature
  for (auto node_iterator = feature_to_node_in_tree_[feature_id].begin(); node_iterator != feature_to_node_in_tree_[feature_id].end() ; ++node_iterator) {
    std::shared_ptr<WUIPMNode> start_node = *node_iterator;
    // Next node towards root
    std::shared_ptr<WUIPMNode> next_node = (*node_iterator)->parent();
    double p_cap = start_node->p_cap();
    double p_proxy = start_node->p_proxy();
    // Until dummy root
    while (next_node && next_node->feature_id() > 0) {
      double temporary_p_cap = p_cap + next_node->temporary_p_cap();
      double temporary_p_proxy = fmax(p_proxy, next_node->temporary_p_proxy());
      // Set projected values
      next_node->temporary_p_cap(temporary_p_cap);
      next_node->temporary_p_proxy(temporary_p_proxy);
      next_node = next_node->parent();
    }
  }
}

// Construct WUIPMTree from database
void WUIPMTree::Construct (double minimum_support_threshold_percentage, double minimum_u_conf, double minimum_wu_conf, std::vector<std::vector<PAIR_INT_DOUBLE> >& udb, std::unordered_map<int, double>& weight) {
  // Set minimum support count
  minimum_support_threshold_percentage_ = minimum_support_threshold_percentage;
  minimum_support_threshold_ = minimum_support_threshold_percentage_ * udb.size();
  minimum_u_conf_ = minimum_u_conf;
  minimum_wu_conf_ = minimum_wu_conf;

  for (auto row_it = udb.begin(); row_it != udb.end(); ++row_it) {
    CalculateExpectedSupportOfItemsForRow(*row_it);
  }

  ResetItemsLessThanThreshold();
  SetOrderOfFeature();

  for (auto row_it = udb.begin(); row_it != udb.end(); ++row_it) {
    InsertRow(*row_it);
  }
}

// Get the interesting patterns
std::vector<std::vector<int> > WUIPMTree::GetInterestingPatterns () {
    std::vector<std::vector<int> > interesting_patterns;

    for (auto feature_iterator = feature_to_node_in_tree_.begin(); feature_iterator != feature_to_node_in_tree_.end(); ++feature_iterator) {
      interesting_patterns.push_back({ feature_iterator->first });
      ProjectTree(feature_iterator->first);
      Mine(feature_to_node_in_tree_, interesting_patterns, std::vector<int>(1, feature_iterator->first));
    }

    return interesting_patterns;
};

// Get expected support cap from current tree
double WUIPMTree::GetCurrentExpectedSupportCapOfFeature (std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > >& feature_to_node_in_tree, int feature_id) {
  double expected_support_cap = 0.0;
  for (auto feature_iterator = feature_to_node_in_tree[feature_id].begin(); feature_iterator != feature_to_node_in_tree[feature_id].end(); ++feature_iterator) {
    expected_support_cap += (*feature_iterator)->temporary_p_cap();
    (*feature_iterator)->temporary_p_cap((*feature_iterator)->temporary_p_cap() * (*feature_iterator)->temporary_p_proxy());
  }

  return expected_support_cap;
}

// Pune tree according to minimum support threshold
std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > WUIPMTree::PruneTree (
  std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > feature_to_node_in_tree, int feature_id) {
  std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > pruned_list;
  for (auto feature_iterator = feature_to_node_in_tree.begin(); feature_iterator != feature_to_node_in_tree.end(); ++feature_iterator) {
    if (order_of_features_[feature_id] >= order_of_features_[feature_iterator->first]) {
      continue;
    }
    double expected_support_cap = GetCurrentExpectedSupportCapOfFeature(feature_to_node_in_tree, feature_iterator->first);

    if (expected_support_cap < minimum_support_threshold_) {
      continue;
    }

    pruned_list[feature_iterator->first] = feature_iterator->second;
  }

  return pruned_list;
}

void WUIPMTree::SetOrderOfFeature () {
  std::vector<std::pair<double, int> > features_and_expected_support;
  for (auto it = expected_support_of_items_.begin(); it != expected_support_of_items_.end(); ++it) {
    features_and_expected_support.push_back(std::make_pair(it->second, it->first));
  }
  // Sort features by expected support
  sort(features_and_expected_support.begin(),features_and_expected_support.end());
  int order = 1;
  for (auto it = features_and_expected_support.begin(); it != features_and_expected_support.end(); ++it) {
    order_of_features_[it->second] = order++;
  }
}

// Mine on projected tree
void WUIPMTree::Mine (
                std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > existing_feature_to_node_in_tree,
                std::vector<std::vector<int> >& interesting_patterns,
                std::vector<int> prefix_pattern
              ) {
  std::unordered_map<int, std::vector<std::shared_ptr<WUIPMNode> > > pruned_feature_list = PruneTree(existing_feature_to_node_in_tree, prefix_pattern.back());
  for (auto feature_iterator = pruned_feature_list.begin(); feature_iterator != pruned_feature_list.end(); ++feature_iterator) {
    std::vector<int> pattern(prefix_pattern);
    pattern.push_back(feature_iterator->first);
    interesting_patterns.push_back(pattern);
    if (pruned_feature_list.size() > 0) {
      Mine(pruned_feature_list, interesting_patterns, pattern);
    }
  }
}
