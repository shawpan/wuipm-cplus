#ifndef WUIPM_CPLUS_WUIPMNODE_H_
#define WUIPM_CPLUS_WUIPMNODE_H_

#include <vector>
#include <iostream>
#include <memory>

class WUIPMNode {
  public:
    WUIPMNode (
      int feature_id = -1,
      double p_cap = 0,
      double p_proxy = 0,
      std::shared_ptr<WUIPMNode> parent = nullptr
    ) : feature_id_(feature_id), p_cap_(p_cap), p_proxy_(p_proxy), parent_(parent) {};

    ~WUIPMNode() {};

    void Print() const;

    double GetPCap() const { return p_cap_; };
    double GetPProxy() const { return p_proxy_; };
    std::shared_ptr<WUIPMNode> GetParent() const { return parent_; };
    std::vector<std::shared_ptr<WUIPMNode> > GetChildren() const { return children_; };

    void SetParent(std::shared_ptr<WUIPMNode> parent) { parent_ = parent; };
    void SetPCap(double p_cap) { p_cap_ = p_cap; };
    void SetPProxy(double p_proxy) { p_proxy_ = p_proxy; };
    void AddChild(std::shared_ptr<WUIPMNode> child) { children_.push_back(child); };
  private:
    // Represents feature, encoded as a number
    // e.g., feature set of a medical diagnosis
    // { leukomia_probabilty, viral_fever_probabilty } is encoded as
    // { 1, 2}
    int feature_id_;
    // Represents the cap value of expected support
    double p_cap_;
    // Represents a proxy probabilty that best represents probabilty
    // of the nth item in an itemset where n > 2
    double p_proxy_;
    std::shared_ptr<WUIPMNode> parent_;
    std::vector<std::shared_ptr<WUIPMNode> > children_;
};

#endif // WUIPM_CPLUS_WUIPMNODE_H_
