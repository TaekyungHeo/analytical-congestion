/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "EventQueue.hh"
#include <iostream>

using namespace Congestion;

EventQueue::EventQueue() noexcept : current_time(0) {}

void EventQueue::insert(Event event, Time event_time) noexcept {
  for (auto it = event_queue.begin(); it != event_queue.end(); it++) {
    auto timestamp = it->get_timestamp();
    if (timestamp == event_time) {
      // insert here
      it->insert(std::move(event));
      return;
    } else if (timestamp > event_time) {
      // should insert here
      auto new_entry = event_queue.emplace(it, event_time);
      new_entry->insert(std::move(event));
      return;
    }
  }

  // reaching here:
  //  (1) no entry at all
  //  (2) all entries smaller than given
  // either way, insert new entry at the end
  auto& new_entry = event_queue.emplace_back(event_time);
  new_entry.insert(std::move(event));
}

void EventQueue::run() noexcept {
  auto& event_queue_entry = event_queue.front();
  current_time = event_queue_entry.get_timestamp();
  event_queue_entry.run();
  event_queue.pop_front();
}

[[nodiscard]] EventQueue::Time EventQueue::get_current_time() const noexcept {
  return current_time;
}

[[nodiscard]] bool EventQueue::empty() const noexcept {
  return event_queue.empty();
}

void EventQueue::print() const noexcept {
  std::cout << "[EventQueue]" << std::endl;
  std::cout << "\tTime: " << current_time << std::endl;
  std::cout << "\tNum Timestamps: " << event_queue.size() << std::endl;
  std::cout << "\t[Entries]" << std::endl;
  for (auto it = event_queue.begin(); it != event_queue.end(); it++) {
    it->print();
  }
  std::cout << "[Done]" << std::endl << std::endl;
  //  std::cout << std::endl;
}
