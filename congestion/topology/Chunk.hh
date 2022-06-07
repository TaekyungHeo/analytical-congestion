/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef CONGESTION_CHUNK_HH
#define CONGESTION_CHUNK_HH

#include <vector>
#include "../event-queue/Event.hh"

namespace Congestion {

class Chunk {
 public:
  using NpuId = int;
  using Size = int;
  using Path = std::vector<NpuId>;
  typedef void (*AstraSimCallback)(void*);

  Chunk(
      Size size,
      Path path,
      AstraSimCallback callback,
      void* callback_arg) noexcept;

  ~Chunk() noexcept = default;

  [[nodiscard]] bool reached_dest() const noexcept;

  [[nodiscard]] NpuId current_npu() const noexcept;

  void proceed() noexcept;

  [[nodiscard]] NpuId next_npu() const noexcept;

  [[nodiscard]] Size get_size() const noexcept;

  void invoke_callback() noexcept;

 private:
  Size size;
  Path path;
  int current_index;
  AstraSimCallback callback;
  void* callback_arg;
};

} // namespace Congestion

#endif
