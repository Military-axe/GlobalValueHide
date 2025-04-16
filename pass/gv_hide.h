#pragma once

#include "collector.h"
#include "encryptor.h"
#include "replacer.h"
#include <cstdint>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalObject.h>
#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <vector>

const uint32_t OPAQUE_PREDICATE_ARGV_MIN = 2;

namespace global_value_hide {

/// @brief Central manager class for global value hiding operations.
/// @details Coordinates collection, encryption, and replacement phases
/// to obfuscate global values in an LLVM module.
class GlobalValHideManager {
private:
  llvm::Module &M_; ///< Reference to the target LLVM module.
  std::unique_ptr<GlobalValueCollector>
      collector_; ///< Collects global values/functions.
  std::unique_ptr<GlobalValueEncryptor>
      encryptor_; ///< Handles value encryption.
  std::unique_ptr<GlobalValueReplacer>
      replacer_; ///< Manages value replacement.

public:
  /// @brief Constructs a manager for the given module.
  /// @param M The LLVM module to obfuscate.
  explicit GlobalValHideManager(llvm::Module &M) : M_(M) {
    collector_ = std::make_unique<GlobalValueCollector>(M_);
    encryptor_ = std::make_unique<GlobalValueEncryptor>(M_);
    replacer_ = std::make_unique<GlobalValueReplacer>(M_.getContext());
  };

  /// @brief Executes the full obfuscation workflow:
  /// 1. Collect global values
  /// 2. Encrypt collected values
  /// 3. Replace original references
  void run();
};

} // namespace global_value_hide

/// @brief Main class implementing global value hiding transformation.
/// @details Performs encryption of global variables and replaces their uses
/// with obfuscated references through a generated global array.
class GlobalValHide {
private:
  std::vector<llvm::ConstantInt *>
      encKey_; ///< Encryption keys for global variables.
  std::vector<llvm::GlobalVariable *> gv_; ///< Collected global variables.

  /// @brief Generates encrypted global variable pointers with random offsets.
  /// @param ctx LLVM context for type creation.
  /// @return std::vectorllvm::Constant* Array of encrypted pointer constants.
  std::vector<llvm::Constant *> encGlobalValue(llvm::LLVMContext &ctx);

  /// @brief Creates a global array storing encrypted variable references.
  /// @param M Target module to modify.
  /// @return llvm::GlobalVariable* Pointer to the created encrypted global
  /// array.
  llvm::GlobalVariable *createEncGlobalValue(llvm::Module &M);

  /// @brief Replaces all uses of original GV with decrypted references.
  /// @param GV Original global variable to replace.
  /// @param encGV Encrypted global array containing references.
  /// @param ctx LLVM context for IR manipulation.
  void replaceGvUsersWithEncGv(llvm::GlobalVariable *GV,
                               llvm::GlobalVariable *encGV,
                               llvm::LLVMContext &ctx);

public:
  /// @brief Constructs a GlobalValHide instance and collects global variables.
  /// @param M The LLVM module to process.
  explicit GlobalValHide(llvm::Module &M);

  /// @brief Main entry point for global value conversion.
  /// @details If global variables exist:
  /// 1. Creates encrypted global array
  /// 2. Replaces all original variable uses
  /// @param M Target module to modify.
  void globalValueConvert(llvm::Module &M);
};