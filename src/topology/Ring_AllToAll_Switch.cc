/******************************************************************************
Copyright (c) 2020 Georgia Institute of Technology
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Author : William Won (william.won@gatech.edu)
*******************************************************************************/

#include <cassert>
#include <cmath>
#include "Ring_AllToAll_Switch.hh"

using namespace Analytical;

Ring_AllToAll_Switch::Ring_AllToAll_Switch(std::vector<TopologyConfiguration> configurations,
                                           std::vector<int> nodes_per_dims) noexcept :
        configurations(configurations), nodes_per_dims(nodes_per_dims) {
    tiles_per_gpu = nodes_per_dims[0];
    half_tiles_per_gpu = tiles_per_gpu / 2;
    tiles_per_node = nodes_per_dims[1] * tiles_per_gpu;
}

Topology::Latency Ring_AllToAll_Switch::simulateSend(NodeID src, NodeID dest, PayloadSize payload_size) noexcept {
    // translate nodeID into address by dimension
    auto src_node = src / tiles_per_node;
    auto src_gpu = (src % tiles_per_node) / tiles_per_gpu;
    auto src_tile = src % tiles_per_gpu;

    auto dest_node = dest / tiles_per_node;
    auto dest_gpu = (dest % tiles_per_node) / tiles_per_gpu;
    auto dest_tile = dest % tiles_per_gpu;


    // If node differs, use scale-out switch
    if (src_node != dest_node) {
        const auto hops_count = 2;

        // compute communication latency
        auto configuration = configurations[2];

        auto comm_latency = hops_count * configuration.getLinkLatency();  // Link delay
        comm_latency += payload_size / configuration.getBandwidth();  // Serialization delay
        comm_latency += configuration.getRouterLatency();  // Switch delay
        comm_latency += 2 * configuration.getNIC_Latency();  // NIC delay when nic is enabled

        // compute hbm delay
        auto hbm_latency = configuration.getHBM_Latency();  // HBM delay
        hbm_latency += payload_size / configuration.getHBM_Bandwidth();  // HBM serialization delay
        hbm_latency *= configuration.getHBM_Scale();  // Scale HBM by scaling factor

        auto latency = std::max((int)comm_latency, (int)hbm_latency);
        assert(latency >= 0 && "[Ring_AllToAll_Switch] Latency error: latency cannot be negative");

        return latency;
    }

    // Use scale-up networks
    auto latency = 0.0;
    if (src_gpu != dest_gpu) {
        // should use all-to-all network
        const auto hops_count = 1;

        // compute communication latency
        auto configuration = configurations[1];

        auto comm_latency = hops_count * configuration.getLinkLatency();  // Link delay
        comm_latency += payload_size / configuration.getBandwidth();  // Serialization delay
        comm_latency += configuration.getRouterLatency();  // Switch delay
        comm_latency += 2 * configuration.getNIC_Latency();  // NIC delay when nic is enabled

        // compute hbm delay
        auto hbm_latency = configuration.getHBM_Latency();  // HBM delay
        hbm_latency += payload_size / configuration.getHBM_Bandwidth();  // HBM serialization delay
        hbm_latency *= configuration.getHBM_Scale();  // Scale HBM by scaling factor

        latency += std::max((int)comm_latency, (int)hbm_latency);
    }

    if (src_tile != dest_tile) {
        // should use ring
        auto hops_count = -1;

        // compute clockwise distance
        auto distance = std::abs(dest_tile - src_tile);
        if (distance > (tiles_per_gpu / 2)) {
            hops_count = tiles_per_gpu - distance;
        } else {
            hops_count = distance;
        }

        // compute communication latency
        auto configuration = configurations[0];

        auto comm_latency = hops_count * configuration.getLinkLatency();  // Link delay
        comm_latency += payload_size / configuration.getBandwidth();  // Serialization delay
        comm_latency += configuration.getRouterLatency();  // Switch delay
        comm_latency += 2 * configuration.getNIC_Latency();  // NIC delay when nic is enabled

        // compute hbm delay
        auto hbm_latency = configuration.getHBM_Latency();  // HBM delay
        hbm_latency += payload_size / configuration.getHBM_Bandwidth();  // HBM serialization delay
        hbm_latency *= configuration.getHBM_Scale();  // Scale HBM by scaling factor

        latency += std::max((int)comm_latency, (int)hbm_latency);
    }

    return latency;
}
