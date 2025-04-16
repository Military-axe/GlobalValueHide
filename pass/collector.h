#pragma once

#include "prelude.h"
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Module.h>
#include <vector>

namespace global_value_hide {

/// @brief Collects global variables and functions from an LLVM module for
/// obfuscation.
/// @details This class is used by GlobalValHideManager to gather all global
/// values required for the obfuscation process.
/// @note Friend class of GlobalValHideManager to allow direct access to
/// collected data.
class GlobalValueCollector {
  friend class GlobalValHideManager;

private:
  llvm::Module &M_;  ///< Reference to the target LLVM module.
  Funcs funcs_;      ///< Collected functions in the module.
  GlobalValues gvs_; ///< Collected global variables in the module.

public:
  /// @brief Constructor for GlobalValueCollector.
  /// @param M The LLVM module to collect global values from.
  explicit GlobalValueCollector(llvm::Module &M) : M_(M) {};

  /// @brief Collects all global variables and functions in the module.
  /// @details Populates funcs_ and gvs_ by iterating through the module's
  /// contents.
  void collect();
};

/// @brief Traits class template for collecting specific types of global values.
/// @tparam T Type of global value to collect (llvm::Function or
/// llvm::GlobalVariable).
template <typename T> struct CollecTrait {
  /// @brief Collects global values of type T from the module.
  /// @param M The LLVM module to collect from.
  /// @return std::vector<T*> containing pointers to the collected values.
  static std::vector<T *> collect(llvm::Module &M);
};

/// @brief Specialization of CollecTrait for collecting llvm::Function objects.
template <> struct CollecTrait<llvm::Function> {

  /// @brief Collects all functions in the module.
  /// @param M The LLVM module to process.
  /// @return std::vectorllvm::Function* containing all function pointers.
  static std::vector<llvm::Function *> collect(llvm::Module &M) {
    Funcs Fs;
    for (auto &F : M) {
      Fs.push_back(&F);
    }

    return Fs;
  }
};

/// @brief Specialization of CollecTrait for collecting llvm::GlobalVariable
/// objects.
template <> struct CollecTrait<llvm::GlobalVariable> {

  /// @brief Collects all global variables in the module.
  /// @param M The LLVM module to process.
  /// @return std::vectorllvm::GlobalVariable* containing all global variable
  /// pointers.
  static GlobalValues collect(llvm::Module &M) {
    GlobalValues GVs;
    for (auto &GV : M.globals()) {
      GVs.push_back(&GV);
    }

    return GVs;
  }
};

/// @brief Helper function to collect global values using CollecTrait.
/// @tparam T Type of global value to collect (llvm::Function or
/// llvm::GlobalVariable).
/// @param M The LLVM module to collect from.
/// @return std::vector<T*> containing pointers to the collected values.
template <typename T> std::vector<T *> Collector(llvm::Module &M) {
  return CollecTrait<T>::collect(M);
}

}; // namespace global_value_hide