#ifndef WUIPM_CPLUS_WUIPM_NODE_H_
#define WUIPM_CPLUS_WUIPM_NODE_H_

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

    void Print(std::string, bool) const;
    void AddChild(std::shared_ptr<WUIPMNode> child) { children_.push_back(child); };

    int feature_id() const { return feature_id_; };
    double p_cap() const { return p_cap_; };
    double p_proxy() const { return p_proxy_; };
    double temporary_p_cap() const { return temporary_p_cap_; };
    double temporary_p_proxy() const { return temporary_p_proxy_; };
    std::shared_ptr<WUIPMNode> parent() const { return parent_; };
    std::vector<std::shared_ptr<WUIPMNode> > children() const { return children_; };
    void parent(std::shared_ptr<WUIPMNode> parent) { parent_ = parent; };
    void p_cap(double p_cap) { p_cap_ = p_cap; };
    void p_proxy(double p_proxy) { p_proxy_ = p_proxy; };
    void temporary_p_cap(double p_cap) { temporary_p_cap_ = p_cap; };
    void temporary_p_proxy(double p_proxy) { temporary_p_proxy_ = p_proxy; };
  private:
    // Represents feature, encoded as a number
    // e.g., feature set of a medical diagnosis
    // { leukomia_probabilty, viral_fever_probabilty } is encoded as
    // { 1, 2}
    int feature_id_;
    // Represents the cap value of expected support
    double p_cap_;
    // This is same as pa_cap_ but will be used while project tree and mining
    // to hold temporarily calculated values
    double temporary_p_cap_;
    // Represents a proxy probabilty that best represents probabilty
    // of the nth item in an itemset where n > 2
    double p_proxy_;
    // It is same as p_proxy_ but will be used while project tree and mining
    // to hold temporarily calculated values
    double temporary_p_proxy_;
    std::shared_ptr<WUIPMNode> parent_;
    std::vector<std::shared_ptr<WUIPMNode> > children_;
};

#endif // WUIPM_CPLUS_WUIPM_NODE_H_
