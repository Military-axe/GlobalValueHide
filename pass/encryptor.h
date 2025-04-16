#pragma once

#include "prelude.h"
#include "utils/utils.h"
#include <llvm/IR/Constant.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/Module.h>

namespace global_value_hide {

/// @brief Encrypts global variables and functions to obfuscate their
/// references.
/// @details Generates encrypted pointers and stores them in a private global
/// array.
/// @note Friend class of GlobalValHideManager for direct access to encryption
/// data.
class GlobalValueEncryptor {
  friend class GlobalValHideManager;

private:
  llvm::Module &M_;  ///< Reference to the target LLVM module.
  EncGvsInfo gv_;    ///< Metadata of encrypted global variables.
  EncFunsInfo func_; ///< Metadata of encrypted functions.

public:
  /// @brief Constructor for GlobalValueEncryptor.
  /// @param M The LLVM module to encrypt values in.
  explicit GlobalValueEncryptor(llvm::Module &M) : M_(M) {};

  /// @brief Encrypts the provided global variables and functions.
  /// @param gv Global variables to encrypt.
  /// @param func Functions to encrypt.
  void enc(GlobalValues &gv, Funcs &func);
};

/// @brief Encryption trait for generating obfuscated pointers.
/// @tparam T Type of value to encrypt (llvm::GlobalVariable or llvm::Function).
template <typename T> class EncTrait {
private:
  /// @brief Creates an encrypted pointer by adding a random key offset.
  /// @param ptr Original pointer to the global value.
  /// @param ctx LLVM context for type creation.
  /// @return llvm::Constant* representing the encrypted pointer expression.
  static llvm::Constant *createEncryptedPointer(llvm::Constant *ptr,
                                                llvm::LLVMContext &ctx) {
    static auto randEngine = utils::RandomEngine();
    auto key = randEngine.getUint64();
    auto keyVal = llvm::ConstantInt::get(llvm::Type::getInt64Ty(ctx), key);

    return llvm::ConstantExpr::getGetElementPtr(
        llvm::Type::getInt8Ty(ctx),
        llvm::ConstantExpr::getBitCast(ptr,
                                 llvm::PointerType::get(llvm::Type::getInt8Ty(ctx), 0)),
        keyVal);
  }

public:
  /// @brief Encrypts a list of global values and creates an encrypted global
  /// array.
  /// @param M Target module to modify.
  /// @param vals List of global values to encrypt.
  /// @return std::vector<EncryptedValue<T>> containing encryption metadata.
  static std::vector<EncryptedValue<T>> enc(llvm::Module &M,
                                            std::vector<T *> &vals) {
    auto int8PtrTy = llvm::PointerType::get(llvm::Type::getInt8Ty(M.getContext()), 0);
    std::vector<llvm::Constant *> encryptedPtrs;
    std::vector<EncryptedValue<T>> encryptedGlobals;

    // encrypt all object in vals
    for (size_t i = 0; i < vals.size(); ++i) {
      auto encPtr = createEncryptedPointer(vals[i], M.getContext());
      encryptedPtrs.push_back(encPtr);
      encryptedGlobals.push_back(
          {nullptr, llvm::dyn_cast<llvm::Constant>(encPtr->getOperand(1)), i, vals[i]});
    }

    // create encrypted global value array
    auto arrTy = llvm::ArrayType::get(int8PtrTy, encryptedPtrs.size());
    auto encGV = new llvm::GlobalVariable(M, arrTy, true, llvm::GlobalValue::PrivateLinkage,
                                    llvm::ConstantArray::get(arrTy, encryptedPtrs),
                                    "__encrypted_globals");

    // updata encrypted global value index in array
    for (auto &ev : encryptedGlobals) {
      ev.encryptedGV = encGV;
    }

    return encryptedGlobals;
  }
};

/// @brief Wrapper function for EncTrait-based encryption.
/// @tparam T Type of value to encrypt.
/// @param M Target module.
/// @param vals List of values to encrypt.
/// @return std::vector<EncryptedValue<T>> containing encryption metadata.
template <typename T>
std::vector<EncryptedValue<T>> Encryptor(llvm::Module &M,
                                         std::vector<T *> &vals) {
  return EncTrait<T>::enc(M, vals);
}

} // namespace global_value_hide