/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Npu.hh"

using namespace Congestion;

void Npu::wakeup(void* npu_data, void* chunk_data) noexcept {
  // this callback gets invoked when a chunk transmission finishes.
  // 1. Proceed one step
  // 2. Process the chunk, using `service` function
  auto npu = static_cast<Npu*>(npu_data);
  auto chunk = static_cast<Chunk*>(chunk_data);

  auto next_npu = chunk->next_npu();
  chunk->proceed();
  npu->get_next_npu(next_npu)->service(chunk);
}

Npu::Npu(Npu::NpuId id) noexcept : id(id) {}

void Npu::service(Chunk* chunk) noexcept {
  // 1. If reached destination, invoke callback
  // 2. If not, schedule this chunk

  if (chunk->reached_dest()) {
    chunk->invoke_callback();
    delete chunk;
  } else {
    auto next_npu = chunk->next_npu();
    links[next_npu]->schedule(wakeup, this, chunk);
  }
}

std::shared_ptr<Npu> Npu::get_next_npu(Npu::NpuId next_id) noexcept {
  return npus[next_id];
}

void Npu::connect(
    Npu::NpuId dest_id,
    std::shared_ptr<Npu> dest_npu,
    Npu::Latency latency,
    Npu::Bandwidth bandwidth) noexcept {
  npus[dest_id] = dest_npu;
  links[dest_id] = std::make_shared<Link>(latency, bandwidth);
}
