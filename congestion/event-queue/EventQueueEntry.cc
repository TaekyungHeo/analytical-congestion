/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "EventQueueEntry.hh"
#include <iostream>

using namespace Congestion;

EventQueueEntry::EventQueueEntry(EventQueueEntry::Time timestamp) noexcept
    : timestamp(timestamp) {}

EventQueueEntry::Time EventQueueEntry::get_timestamp() const noexcept {
  return timestamp;
}

void EventQueueEntry::insert(Event event) noexcept {
  events.push_back(std::move(event));
}

void EventQueueEntry::run() noexcept {
  while (!events.empty()) {
    auto& event = events.front();
    event.invoke_callback();
    events.pop_front();
  }
}

void EventQueueEntry::print() const noexcept {
  std::cout << "\t\tTimestamp " << timestamp << ": " << events.size()
            << " events" << std::endl;
}