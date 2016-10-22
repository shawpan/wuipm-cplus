#include "wuipm_node.h"

void WUIPMNode::Print () const {
  std::cout << "------------------------" << std::endl;
  std::cout << "Printing WUIPM Node ..." << std::endl;
  std::cout << "Feature Id: " << feature_id_ << std::endl;
  std::cout << "pCap: " << p_cap_ << std::endl;
  std::cout << "pProxy: " << p_proxy_ << std::endl;
  std::cout << "Parent exists? : " << (parent_? "yes" : "no") << std::endl;
  std::cout << "Number of children : " << children_.size() << std::endl;
  std::cout << "------------------------" << std::endl;
}
