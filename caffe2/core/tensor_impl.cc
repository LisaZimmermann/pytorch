#include "caffe2/core/tensor_impl.h"
#include "caffe2/core/context_base.h"
#include "caffe2/core/flags.h"

CAFFE2_DEFINE_bool(
    caffe2_keep_on_shrink,
    true,
    "If set, keeps memory when a tensor is shrinking its size.");

CAFFE2_DEFINE_int64(
    caffe2_max_keep_on_shrink_memory,
    LLONG_MAX,
    "The maximum memory in bytes to keep on shrink, if the difference between "
    "tensor sizes is bigger than this then tensor will be reset.");

namespace caffe2 {
static void deletePlacementDeleteContext(void* ptr) {
  delete static_cast<PlacementDeleteContext*>(ptr);
}

at::DataPtr PlacementDeleteContext::makeDataPtr(
    Ctx&& prev_ctx,
    PlacementDtor placement_dtor,
    size_t size,
    at::Device device) {
  auto* ptr = prev_ctx.get();
  return {ptr,
          new PlacementDeleteContext(std::move(prev_ctx), placement_dtor, size),
          &deletePlacementDeleteContext,
          device};
}
} // namespace caffe2
