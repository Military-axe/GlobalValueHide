#include "sub2.h"
#include "utils/utils.h"
#include <cstdint>

using namespace llvm;

namespace global_value_hide {

Value *Sub2::substitution(llvm::IRBuilder<> &IRB, llvm::Value *encrypted,
                          llvm::Constant *key, llvm::LLVMContext &ctx) {
  static utils::RandomEngine randomEngine;
  volatile uint32_t randA = randomEngine.getUint32();
  volatile uint32_t randB = randomEngine.getUint32();
  volatile uint32_t randC = randomEngine.getRange(UINT16_MAX, randB - 1);
  auto randD = randA + randB - randC;

  Value *keyVal = IRB.CreatePtrToInt(key, Type::getInt64Ty(ctx));
  auto key_fine = IRB.CreateAdd(keyVal, IRB.getInt64(randA));
  key_fine = IRB.CreateSub(key_fine, IRB.getInt64(randC));
  key_fine = IRB.CreateAdd(key_fine, IRB.getInt64(randB));
  key_fine = IRB.CreateAdd(key_fine, IRB.getInt64(randD));

  auto gvAddr =
      IRB.CreateGEP(encrypted->getType(), encrypted, key_fine, "sub2_gef");
  return gvAddr;
}

} // namespace global_value_hide