/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef CONGESTION_EVENTQUEUE_HH
#define CONGESTION_EVENTQUEUE_HH

#include <list>
#include "Event.hh"
#include "EventQueueEntry.hh"

namespace Congestion {

class EventQueue {
 public:
  using Time = EventQueueEntry::Time;

  EventQueue() noexcept;
  ~EventQueue() noexcept = default;

  void insert(Event event, Time event_time) noexcept;

  void run() noexcept;

  [[nodiscard]] Time get_current_time() const noexcept;

  [[nodiscard]] bool empty() const noexcept;

  void print() const noexcept;

 private:
  std::list<EventQueueEntry> event_queue;
  Time current_time;
};

} // namespace Congestion

#endif
