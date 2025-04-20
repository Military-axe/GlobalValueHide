#include "sub1.h"
#include "utils/utils.h"

using namespace llvm;

namespace global_value_hide {

Value *Sub1::substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                          llvm::Constant *key, llvm::LLVMContext &ctx) {
  static auto randEngine = utils::RandomEngine();

  volatile uint32_t randA = randEngine.getUint32();
  volatile uint32_t randB = randEngine.getUint32();
  volatile uint32_t randC = randEngine.getUint32();
  volatile uint32_t randD = randEngine.getUint32();

  // constA - ((-(key + constA)) + ((key + constA) ^ constB) + constC) ^ constD)
  Value *base = IRB.CreatePtrToInt(encrypted, Type::getInt64Ty(ctx));
  Value *keyVal = IRB.CreatePtrToInt(key, Type::getInt64Ty(ctx));

  // use rand number
  Value *calc1 = IRB.CreateAdd(keyVal, IRB.getInt64(randA));
  calc1 = IRB.CreateXor(calc1, IRB.getInt64(randB));
  Value *calc2 = IRB.CreateSub(IRB.getInt64(randC), calc1);
  calc2 = IRB.CreateAnd(calc2, IRB.getInt64(randD));

  Value *dynamicOffset = IRB.CreateAdd(base, calc2);
  dynamicOffset = IRB.CreateTrunc(dynamicOffset, Type::getInt32Ty(ctx));

  //=== Anti-optimization reinforcement ===//
  // Tip 1: Insert fake memory dependencies
  Value *dummyPtr =
      IRB.CreateIntToPtr(base, PointerType::get(Type::getInt8Ty(ctx), 0));
  LoadInst *volatileLoad = IRB.CreateLoad(Type::getInt8Ty(ctx), dummyPtr);
  volatileLoad->setVolatile(true);
  dynamicOffset = IRB.CreateAdd(
      dynamicOffset, IRB.CreateZExt(volatileLoad, Type::getInt32Ty(ctx)));

  // Tip 2: Create unpredictable sequences of instructions
  for (int i = 0; i < 3; ++i) {
    dynamicOffset =
        IRB.CreateXor(dynamicOffset, IRB.getInt32(randEngine.getUint32()));
    dynamicOffset = IRB.CreateAdd(dynamicOffset, IRB.getInt32(1));
  }

  Value *gvAddr = IRB.CreateGEP(encrypted->getType(), encrypted,
                                dynamicOffset, // 使用动态计算的偏移
                                "obf_gep");

  if (Instruction *gepInst = dyn_cast<Instruction>(gvAddr)) {
    MDNode *meta = MDNode::get(ctx, {MDString::get(ctx, "obfuscated")});
    gepInst->setMetadata("obf.md", meta);
  }

  return gvAddr;
}

} // namespace global_value_hide