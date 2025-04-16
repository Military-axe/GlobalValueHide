#pragma once

#include <llvm/IR/GlobalValue.h>
#include <llvm/IR/Type.h>

namespace global_value_hide {

/// @brief A template struct representing an encrypted value.
///
/// This struct holds information about an encrypted global variable or
/// function, including the encrypted global variable, the encryption key, the
/// index in the global value list, and the original global variable or
/// function.
/// @tparam T The type of the original value (either llvm::GlobalVariable or
/// llvm::Function).
template <typename T> struct EncryptedValue {
  /// @brief Pointer to the encrypted global variable or function.
  llvm::GlobalVariable *encryptedGV;
  /// @brief Pointer to the encryption key.
  llvm::Constant *encryptionKey;
  /// @brief Index of the encrypted value in the global value list.
  size_t index;
  /// @brief Pointer to the original global variable or function.
  T *originalValue;
};

using Funcs = std::vector<llvm::Function *>;
using GlobalValues = std::vector<llvm::GlobalVariable *>;
using EncGv = EncryptedValue<llvm::GlobalVariable>;
using EncFun = EncryptedValue<llvm::Function>;
using EncGvsInfo = std::vector<EncryptedValue<llvm::GlobalVariable>>;
using EncFunsInfo = std::vector<EncryptedValue<llvm::Function>>;

} // namespace global_value_hide