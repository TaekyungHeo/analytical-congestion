/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <iostream>
#include <vector>
#include "event-queue/EventQueue.hh"
#include "topology/Chunk.hh"
#include "topology/Link.hh"
#include "topology/Npu.hh"

using namespace Congestion;

void callback(void* event_queue_data) {
//  auto event_queue = static_cast<EventQueue*>(event_queue_data);
//  std::cout << "Chunk arrived at: " << event_queue->get_current_time()
//            << std::endl;
}

int main() {
  auto event_queue = std::make_shared<EventQueue>();
  Link::set_event_queue(event_queue);

  auto npus_count = 512;
  auto latency = 500;
  auto bandwidth = 107;
  auto size = 1048576;
  auto npus = std::vector<std::shared_ptr<Npu>>();
  for (auto i = 0; i < npus_count; i++) {
    npus.push_back(std::make_shared<Npu>(i));
  }

  for (int i = 0; i < (npus_count - 1); i++) {
    npus[i]->connect(i + 1, npus[i + 1], latency, bandwidth);
  }
  npus[npus_count - 1]->connect(0, npus[0], latency, bandwidth);

  for (int i = 0; i < npus_count; i++) {
    for (int j = 0; j < npus_count; j++) {
      if (i == j) {
        continue;
      }

      std::vector<Npu::NpuId> path;
      auto t = i;
      path.push_back(t);
      while (true) {
        t++;
        t %= npus_count;
        path.push_back(t);
        if (t == j) {
          break;
        }
      }

      auto* c = new Chunk(size, path, callback, event_queue.get());
      npus[i]->service(c);
    }
  }

  while (!event_queue->empty()) {
    //    event_queue->print();
    event_queue->run();
  }

  auto no_congestion = (npus_count - 1) * (size / bandwidth + latency);
  auto congestion = event_queue->get_current_time();
  auto slowdown = (double)(congestion) / no_congestion;
//  std::cout << std::endl;
//  std::cout << "No congestion: " << no_congestion << std::endl;
//  std::cout << "Congestion: " << congestion << std::endl;
//  std::cout << "Difference: " << slowdown << "x" << std::endl;
  std::cout << no_congestion << std::endl;
  std::cout << congestion << std::endl;

  return 0;
}
