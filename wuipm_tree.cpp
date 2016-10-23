#include "wuipm_tree.h"

void WUIPMTree::PrintTree(std::shared_ptr<WUIPMNode> root, std::string prefix, bool details = false) const {
  root->Print(prefix, details);
  std::vector<std::shared_ptr<WUIPMNode> > children = root->GetChildren();
  for (size_t i = 0; i < children.size(); i++) {
    PrintTree(children[i], prefix + "    ", details);
  }
}

void WUIPMTree::Print (bool details = false) const {
  std::cout << "Printing WUIPM Tree" << std::endl;
  std::cout << "------------------------" << std::endl;
  PrintTree(root_, "", details);
}

void WUIPMTree::InsertRow (std::unordered_map<int, double> row) {
  std::shared_ptr<WUIPMNode> current_root = root_;
  for (auto it = row.begin(); it != row.end(); it++) {
    std::vector<std::shared_ptr<WUIPMNode> > children = current_root->GetChildren();
    bool child_found = false;
    for (size_t j = 0; j < children.size(); j++) {
      // If branch is matched break
      if (children[j]->GetFeatureId() == it->first) {
        current_root = children[j];
        child_found = true;
        break;
      }
    }
    // If branch is not matched
    if (! child_found) {
      std::shared_ptr<WUIPMNode> new_child = std::make_shared<WUIPMNode>(it->first);
      current_root->AddChild(new_child);
      new_child->SetParent(current_root);
      current_root = new_child;
    }
  }
}

void WUIPMTree::Construct () {
  InsertRow({ {1 , 0.4}, {2, 0.5} });
  InsertRow({ {1 , 0.4}, {3, 0.5} });
}
