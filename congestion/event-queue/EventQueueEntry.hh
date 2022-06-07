/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef CONGESTION_EVENTQUEUEENTRY_HH
#define CONGESTION_EVENTQUEUEENTRY_HH

#include <list>
#include "Event.hh"

namespace Congestion {

class EventQueueEntry {
 public:
  using Time = int;

  EventQueueEntry(Time timestamp) noexcept;

  ~EventQueueEntry() noexcept = default;

  [[nodiscard]] Time get_timestamp() const noexcept;

  void insert(Event event) noexcept;

  void run() noexcept;

  void print() const noexcept;

 private:
  Time timestamp;
  std::list<Event> events;
};

} // namespace Congestion

#endif
