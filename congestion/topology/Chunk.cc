/******************************************************************************
This source code is licensed under the MIT license found in the
LICENSE file in the root directory of this source tree.
*******************************************************************************/

#include <utility>

#include <iostream>
#include "Chunk.hh"

using namespace Congestion;

Chunk::Chunk(
    Size size,
    Path path,
    Chunk::AstraSimCallback callback,
    void* callback_arg) noexcept
    : size(size),
      path(std::move(path)),
      current_index(0),
      callback(callback),
      callback_arg(callback_arg) {}

bool Chunk::reached_dest() const noexcept {
  return current_index >= (path.size() - 1);
}

Chunk::NpuId Chunk::current_npu() const noexcept {
  return path[current_index];
}

Chunk::NpuId Chunk::next_npu() const noexcept {
  assert(!reached_dest());

  return path[current_index + 1];
}

void Chunk::proceed() noexcept {
  assert(!reached_dest());

  current_index++;
}

Chunk::Size Chunk::get_size() const noexcept {
  return size;
}

void Chunk::invoke_callback() noexcept {
  (*callback)(callback_arg);
}
