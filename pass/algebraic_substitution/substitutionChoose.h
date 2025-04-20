#include "substitution.h"
#include <vector>

namespace global_value_hide {

using AlgSubList = std::vector<std::unique_ptr<AlgebraicSubstitutionInterface>>;

class AlgebraicSubstitutionChoose {
private:
  AlgSubList subs_;
  AlgSubList collectorSubstitution();

public:
  AlgebraicSubstitutionChoose();
  AlgebraicSubstitutionInterface& choose();
};

} // namespace global_value_hide