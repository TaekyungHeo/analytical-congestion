/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include "Event.hh"

using namespace Congestion;

Event::Event(Callback callback, void* arg1, void* arg2) noexcept
    : callback(callback), arg1(arg1), arg2(arg2) {}

void Event::invoke_callback() noexcept {
  (*callback)(arg1, arg2);
}
