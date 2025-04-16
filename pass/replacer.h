#pragma once

#include "prelude.h"
#include <llvm/IR/Function.h>
#include <llvm/IR/GlobalVariable.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/User.h>
#include <llvm/Support/Casting.h>
#include <llvm/Support/raw_ostream.h>
#include <sys/stat.h>

namespace global_value_hide {

/// @brief A class for replacing global values with encrypted counterparts.
///
/// This class handles the replacement of global variables and functions
/// by decrypting their addresses at runtime using dynamically generated keys.
/// It is designed to work with LLVM's IRBuilder for instruction insertion.
class GlobalValueReplacer {
  friend class GlobalValHideManager;

private:
  /// @brief A reference to the LLVM context used for IR modifications.
  llvm::LLVMContext &ctx_;

public:
  /// @brief Constructs a GlobalValueReplacer with the given LLVM context.
  ///
  /// @param ctx The LLVM context associated with the current module.
  GlobalValueReplacer(llvm::LLVMContext &ctx) : ctx_(ctx) {};

  /// @brief Replaces encrypted global values and functions in the IR.
  ///
  /// Iterates through all provided encrypted global variables (gvs) and
  /// functions (funcs), replacing their uses with dynamically decrypted values.
  ///
  /// @param gvs   Container of encrypted global variable metadata.
  /// @param funcs Container of encrypted function metadata.
  void replace(const EncGvsInfo &gvs, const EncFunsInfo &funcs);
};

/// @brief Decrypts an encrypted value using a runtime-generated key.
///
/// Generates a complex decryption sequence involving arithmetic operations,
/// random constants, and anti-optimization techniques to obscure the
/// decryption logic. The result is a dynamically computed address.
///
/// @param IRB       IRBuilder for inserting decryption instructions.
/// @param encrypted Encrypted value (global variable or function address).
/// @param key       Constant encryption key used in the decryption formula.
/// @param ctx       LLVM context for type creation.
/// @return          Value* representing the decrypted address.
///
/// @note Implements anti-optimization measures:
/// - Fake memory dependencies via volatile loads
/// - Unpredictable instruction sequences with random XOR/ADD operations
llvm::Value *decryptValue(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                          llvm::Constant *key, llvm::LLVMContext &ctx);

/// @brief Template trait for replacing specific types of encrypted values.
///
/// Provides a generic interface for replacing uses of encrypted globals.
/// Specializations must implement the static `replace` method.
///
/// @tparam T Type of value to replace (e.g., GlobalVariable, Function).
template <typename T> struct ReplaceTrait {
  /// @brief Replaces all uses of an encrypted value with its decrypted form.
  ///
  /// @param ctx LLVM context for IR modifications.
  /// @param ev  Metadata containing the original value, encrypted GV, and key.
  static void replace(llvm::LLVMContext &ctx, const EncryptedValue<T> &ev);
};

/// @brief Specialization of ReplaceTrait for GlobalVariable replacement.
///
/// Replaces all uses of a global variable with a GEP-load-decrypt sequence.
/// Inserts metadata ("obf.md") on the generated GEP instructions for analysis.
template <> struct ReplaceTrait<llvm::GlobalVariable> {
  /// @brief Replaces uses of an encrypted global variable.
  ///
  /// For each user of the original GV:
  /// 1. Generates a GEP to access the encrypted GV's slot.
  /// 2. Loads the encrypted address from the GV.
  /// 3. Calls decryptValue to compute the real address.
  /// 4. Replaces the original GV use with the decrypted address.
  ///
  /// @param ctx LLVM context for type creation.
  /// @param ev  EncryptedGlobalVar metadata with index, key, and GV pointers.
  static void replace(llvm::LLVMContext &ctx, const EncGv &ev) {
    auto encGV = ev.encryptedGV;

    for (auto user : ev.originalValue->users()) {
      if (auto *inst = llvm::dyn_cast<llvm::Instruction>(user)) {
        llvm::IRBuilder<> IRB(inst);

        llvm::Value *indices[] = {
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0),
            llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), ev.index)};
        auto gep = IRB.CreateGEP(encGV->getValueType(), encGV, indices);
        llvm::Value *encrypted = IRB.CreateLoad(
            llvm::PointerType::get(llvm::Type::getInt8Ty(ctx), 0), gep,
            ev.originalValue->getName() + "__encrypted");
        auto gvAddr = decryptValue(IRB, encrypted, ev.encryptionKey, ctx);

        inst->replaceUsesOfWith(ev.originalValue, gvAddr);
      }
    }
  }
};

/// @brief Specialization of ReplaceTrait for Function replacement.
///
/// Replaces all call sites of a function with a decrypted function pointer.
/// Handles bitcasting to ensure correct function type signatures.
template <> struct ReplaceTrait<llvm::Function> {
  /// @brief Replaces calls to an encrypted function.
  ///
  /// For each call instruction targeting the original function:
  /// 1. Generates a GEP to access the encrypted function's slot.
  /// 2. Loads the encrypted function pointer.
  /// 3. Decrypts and bitcasts the pointer to the correct function type.
  /// 4. Updates the call's callee to the decrypted function.
  ///
  /// @param ctx LLVM context for type creation.
  /// @param ev  EncryptedFunction metadata with index, key, and GV pointers.
  static void replace(llvm::LLVMContext &ctx, const EncFun &ev) {
    auto encGV = ev.encryptedGV;
    auto key = ev.encryptionKey;

    llvm::SmallVector<llvm::CallInst *, 8> callsToReplace;
    for (auto user : ev.originalValue->users()) {
      if (auto *call = llvm::dyn_cast<llvm::CallInst>(user)) {
        callsToReplace.push_back(call);
      }
    }

    for (auto call : callsToReplace) {
      llvm::IRBuilder<> IRB(call);
      llvm::Value *indices[] = {
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), 0),
          llvm::ConstantInt::get(llvm::Type::getInt32Ty(ctx), ev.index)};

      auto gep = IRB.CreateInBoundsGEP(encGV->getValueType(), encGV, indices);
      llvm::Value *encrypted =
          IRB.CreateLoad(llvm::PointerType::get(llvm::Type::getInt8Ty(ctx), 0),
                         gep, ev.originalValue->getName() + "_encrypted");

      auto decrypted = decryptValue(IRB, encrypted, key, ctx);
      auto funcPtr = IRB.CreateBitCast(
          decrypted, ev.originalValue->getFunctionType()->getPointerTo());

      call->setCalledOperand(funcPtr);
    }
  }
};

}; // namespace global_value_hide