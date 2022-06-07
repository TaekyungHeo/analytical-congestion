/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef CONGESTION_NPU_HH
#define CONGESTION_NPU_HH

#include <map>
#include "Chunk.hh"
#include "Link.hh"

namespace Congestion {

class Npu {
 public:
  using NpuId = Chunk::NpuId;
  using Latency = Link::Latency;
  using Bandwidth = Link::Bandwidth;

  static void wakeup(void* npu_data, void* chunk_data) noexcept;

  Npu(NpuId id) noexcept;

  ~Npu() noexcept = default;

  void service(Chunk* chunk) noexcept;

  void connect(
      Npu::NpuId dest_id,
      std::shared_ptr<Npu> dest_npu,
      Latency latency,
      Bandwidth bandwidth) noexcept;

  [[nodiscard]] std::shared_ptr<Npu> get_next_npu(NpuId id) noexcept;

 private:
  NpuId id;
  std::map<NpuId, std::shared_ptr<Link>> links;
  std::map<NpuId, std::shared_ptr<Npu>> npus;
};

} // namespace Congestion

#endif
