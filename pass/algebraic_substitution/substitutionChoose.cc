#include "substitutionChoose.h"
#include "sub1.h"
#include <memory>
#include <random>

namespace global_value_hide {

AlgSubList AlgebraicSubstitutionChoose::collectorSubstitution() {
  // Add new substitution here
  AlgSubList subs;
  subs.emplace_back(std::make_unique<Sub1>());

  return subs;
}

AlgebraicSubstitutionChoose::AlgebraicSubstitutionChoose() {
  subs_ = collectorSubstitution();
}

AlgebraicSubstitutionInterface& AlgebraicSubstitutionChoose::choose() {
  // choose random substitution
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, subs_.size() - 1);
  int randomIndex = dis(gen);

  if (subs_.empty()) {
    throw std::out_of_range("AlgebraicSubstitution is empty");
  }

  return *subs_[randomIndex];
}

} // namespace global_value_hide