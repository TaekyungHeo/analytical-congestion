/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Link.hh"

using namespace Congestion;

std::shared_ptr<EventQueue> Link::event_queue;

void Link::wakeup(void* link_data, void* chunk_data) noexcept {
  auto link = static_cast<Link*>(link_data);
  if (link->pending_chunk_exists()) {
    link->process_next_chunk();
  } else {
    link->free();
  }
}

void Link::set_event_queue(std::shared_ptr<EventQueue> event_queue) noexcept {
  Link::event_queue = event_queue;
}

Link::Link(Link::Latency latency, Link::Bandwidth bandwidth) noexcept
    : busy(false), latency(latency), bandwidth(bandwidth) {}

void Link::schedule(
    Callback callback,
    void* npu_data,
    void* chunk_data) noexcept {
  // scheduling new chunk_data
  // 1. if already busy, just push it to the queue
  // 2. if not busy, immediately service it.
  if (busy) {
    pending_chunks.push(std::make_tuple(callback, npu_data, chunk_data));
  } else {
    service(callback, npu_data, chunk_data);
  }
}

void Link::service(
    Callback callback,
    void* npu_data,
    void* chunk_data) noexcept {
  // assign two events: LinkBecomeFree and ChunkArrivedNextDest
  // LinkBecomeFree: after serialization delay (bn)
  // ChunkArrivedNextDest: after transmission done (a + bn)
  busy = true;

  auto chunk = static_cast<Chunk*>(chunk_data);

  auto link_become_free = Event(wakeup, this, nullptr);
  auto chunk_arrived_next_dest = Event(callback, npu_data, chunk_data);

  auto current_time = Link::event_queue->get_current_time();
  auto link_free_time = current_time + serialization_delay(chunk->get_size());
  auto chunk_arrival_time = link_free_time + latency;

  Link::event_queue->insert(link_become_free, link_free_time);
  Link::event_queue->insert(chunk_arrived_next_dest, chunk_arrival_time);
}

void Link::free() noexcept {
  busy = false;
}

[[nodiscard]] bool Link::pending_chunk_exists() const noexcept {
  return !pending_chunks.empty();
}

void Link::process_next_chunk() noexcept {
  // TODO: arbitration depending on chunk size, not just FIFO scheduling?
  auto [callback, npu_data, chunk_data] = pending_chunks.front();
  service(callback, npu_data, chunk_data);
  pending_chunks.pop();
}

Link::Latency Link::serialization_delay(Link::Size size) const noexcept {
  return size / bandwidth;
}
