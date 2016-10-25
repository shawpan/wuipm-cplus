#include "wuipm_node.h"

void WUIPMNode::Print (std::string prefix = "", bool details = false) const {
  std::cout << prefix << "------------------------" << std::endl;
  if (details) std::cout << prefix << "Printing WUIPM Node ..." << std::endl;
  if (details) std::cout << prefix << "Feature Id: " << feature_id_ << std::endl;
  if (details) std::cout << prefix << "pCap: " << p_cap_ << std::endl;
  if (details) std::cout << prefix << "Temporary pCap: " << temporary_p_cap_ << std::endl;
  if (details) std::cout << prefix << "pProxy: " << p_proxy_ << std::endl;
  if (details) std::cout << prefix << "Temporary pProxy: " << temporary_p_proxy_ << std::endl;
  if (details) std::cout << prefix << "Parent exists? : " << (parent_? "yes" : "no") << std::endl;
  if (details) std::cout << prefix << "Number of children : " << children_.size() << std::endl;
  if (! details) {
    std::cout << prefix << "( " << feature_id_ << ", ";
    std::cout << p_cap_ << ", ";
    std::cout << p_proxy_ <<" ) ( ";
    std::cout << temporary_p_cap_ << ", ";
    std::cout << temporary_p_proxy_ << " )";
    std::cout<<std::endl;
  }
}
