#include "encryptor.h"
#include "prelude.h"

using namespace llvm;

namespace global_value_hide {

void GlobalValueEncryptor::enc(GlobalValues &gv, Funcs &func) {
  gv_ = Encryptor<GlobalVariable>(M_, gv);
  func_ = Encryptor<Function>(M_, func);
}

} // namespace global_value_hide