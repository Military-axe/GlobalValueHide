#include "collector.h"

namespace global_value_hide {

void GlobalValueCollector::collect() {
  gvs_ = Collector<llvm::GlobalVariable>(M_);
  funcs_ = Collector<llvm::Function>(M_);
}

} // namespace global_value_hide