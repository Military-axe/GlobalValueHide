#include "sub1.h"
#include "utils/utils.h"

using namespace llvm;

namespace global_value_hide {

/// @brief Implements substitution with dynamic obfuscation patterns
/// @details Constructs an obscured pointer calculation using:
/// 1. Random constants generation
/// 2. Volatile memory operations
/// 3. Instruction sequence randomization
/// 4. Metadata tagging for identification
///
/// @param IRB Active IR builder for instruction insertion
/// @param encrypted Pointer value to obfuscate
/// @param key Constant key used in transformation
/// @param ctx LLVM context for type creation
/// @return Obfuscated GEP instruction with anti-analysis properties
Value *Sub1::substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                          llvm::Constant *key, llvm::LLVMContext &ctx) {
  // Random constants pool (volatile to prevent constant propagation)
  static auto randEngine = utils::RandomEngine();
  volatile uint32_t randA = randEngine.getUint32();
  volatile uint32_t randB = randEngine.getUint32();
  volatile uint32_t randC = randEngine.getUint32();
  volatile uint32_t randD = randEngine.getUint32();

  /* Core transformation pipeline */
  // Convert pointers to integer types for arithmetic operations
  Value *base = IRB.CreatePtrToInt(encrypted, Type::getInt64Ty(ctx));
  Value *keyVal = IRB.CreatePtrToInt(key, Type::getInt64Ty(ctx));

  // Construct complex calculation using random constants
  Value *calc1 = IRB.CreateAdd(keyVal, IRB.getInt64(randA));
  calc1 = IRB.CreateXor(calc1, IRB.getInt64(randB));
  Value *calc2 = IRB.CreateSub(IRB.getInt64(randC), calc1);
  calc2 = IRB.CreateAnd(calc2, IRB.getInt64(randD));

  Value *dynamicOffset = IRB.CreateAdd(base, calc2);
  dynamicOffset = IRB.CreateTrunc(dynamicOffset, Type::getInt32Ty(ctx));

  //=== Anti-optimization Measures ===//
  // Insert volatile load from random address to create false dependency
  Value *dummyPtr =
      IRB.CreateIntToPtr(base, PointerType::get(Type::getInt8Ty(ctx), 0));
  LoadInst *volatileLoad = IRB.CreateLoad(Type::getInt8Ty(ctx), dummyPtr);
  volatileLoad->setVolatile(true);
  dynamicOffset = IRB.CreateAdd(
      dynamicOffset, IRB.CreateZExt(volatileLoad, Type::getInt32Ty(ctx)));

  // Add unpredictable instruction sequence
  for (int i = 0; i < 3; ++i) {
    dynamicOffset =
        IRB.CreateXor(dynamicOffset, IRB.getInt32(randEngine.getUint32()));
    dynamicOffset = IRB.CreateAdd(dynamicOffset, IRB.getInt32(1));
  }

  // Create final obfuscated GEP with metadata
  Value *gvAddr =
      IRB.CreateGEP(encrypted->getType(), encrypted, dynamicOffset, "obf_gep");
  if (Instruction *gepInst = dyn_cast<Instruction>(gvAddr)) {
    MDNode *meta = MDNode::get(ctx, {MDString::get(ctx, "obfuscated")});
    gepInst->setMetadata("obf.md", meta);
  }

  return gvAddr;
}

} // namespace global_value_hide