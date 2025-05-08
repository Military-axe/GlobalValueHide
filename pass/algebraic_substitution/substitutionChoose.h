#include "substitution.h"
#include <vector>

namespace global_value_hide {

using AlgSubList = std::vector<std::unique_ptr<AlgebraicSubstitutionInterface>>;

/// @brief Factory class for managing algebraic substitution strategies
///
/// Collects and provides random selection of available algebraic substitution
/// implementations.
class AlgebraicSubstitutionChoose {
private:
  AlgSubList subs_; ///< List of registered substitution strategies

  /// @brief Factory method that collects all available substitution
  /// implementations
  /// @return AlgSubList List containing concrete substitution strategy
  /// instances
  AlgSubList collectorSubstitution();

public:
  /// @brief Constructs substitution chooser and initializes strategy list
  AlgebraicSubstitutionChoose();

  /// @brief Randomly selects an algebraic substitution strategy
  /// @return AlgebraicSubstitutionInterface& Reference to selected strategy
  /// @note Uses thread-local random number generator internally. The returned
  ///       reference remains valid for the lifetime of the chooser object.
  AlgebraicSubstitutionInterface &choose();
};

} // namespace global_value_hide