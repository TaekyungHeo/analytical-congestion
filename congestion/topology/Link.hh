/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef CONGESTION_LINK_HH
#define CONGESTION_LINK_HH

#include <queue>
#include "../event-queue/Event.hh"
#include "../event-queue/EventQueue.hh"
#include "Chunk.hh"

namespace Congestion {

class Link {
 public:
  using Callback = Event::Callback;
  using Latency = int; // ns
  using Bandwidth = int; // B/ns
  using Size = Chunk::Size;

  static void wakeup(void* link_data, void* chunk_data) noexcept;

  static void set_event_queue(std::shared_ptr<EventQueue> event_queue) noexcept;

  Link(Latency latency, Bandwidth bandwidth) noexcept;

  ~Link() noexcept = default;

  void schedule(Callback callback, void* npu_data, void* chunk_data) noexcept;

  void free() noexcept;

  [[nodiscard]] bool pending_chunk_exists() const noexcept;

  void process_next_chunk() noexcept;

 private:
  static std::shared_ptr<EventQueue> event_queue;

  std::queue<std::tuple<Callback, void*, void*>> pending_chunks;
  bool busy;
  Latency latency;
  Bandwidth bandwidth;

  void service(Callback callback, void* npu, void* chunk) noexcept;

  Latency serialization_delay(Size size) const noexcept;
};

} // namespace Congestion

#endif
