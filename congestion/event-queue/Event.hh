/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#ifndef CONGESTION_EVENT_HH
#define CONGESTION_EVENT_HH

namespace Congestion {

class Event {
 public:
  typedef void (*Callback)(void*, void*);

  Event(Callback callback, void* arg1, void* arg2) noexcept;

  ~Event() noexcept = default;

  void invoke_callback() noexcept;

 private:
  Callback callback;
  void* arg1;
  void* arg2;
};

} // namespace Congestion

#endif
