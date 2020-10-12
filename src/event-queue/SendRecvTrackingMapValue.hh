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

#ifndef __SENDRECVTRACKINGMAPVALUE_HH__
#define __SENDRECVTRACKINGMAPVALUE_HH__

#include "Event.hh"
#include "EventQueueEntry.hh"
#include "astra-sim/system/AstraNetworkAPI.hh"

namespace Analytical {
    class SendRecvTrackingMapValue {
    public:
        /**
         * Constructor for send operation
         * @param send_finish_time send operation finish time
         * @return instance with send operation set
         */
        static SendRecvTrackingMapValue make_send_value(AstraSim::timespec_t send_finish_time) noexcept;

        /**
         * Constructor for recv operation
         * @param fun_ptr recv event handler
         * @param fun_arg recv event handler argument
         * @return instance with recv operation set
         */
        static SendRecvTrackingMapValue make_recv_value(void (*fun_ptr)(void *), void *fun_arg) noexcept;

        /**
         * Check whether the operation is send.
         * @return true if operation is send, false if operation is recv
         */
        bool is_send() const noexcept;

        /**
         * Check whether the operation is recv.
         * @return true if operation is recv, false if operation is send
         */
        bool is_recv() const noexcept;

        /**
         * send_finish_time getter
         * @return send_finish_timme
         */
        AstraSim::timespec_t get_send_finish_time() const noexcept;

        /**
         * recv_event getter
         * @return recv_event
         */
        Event get_recv_event() const noexcept;

    private:
        enum class OperationType {
            send,
            recv
        };
        OperationType operation_type;

        /**
         * For send operation: mark when send should finish
         * Note: this is an actual time of event_queue, not delta
         */
        AstraSim::timespec_t send_finish_time;

        /**
         * For recv operation: save receive event handler
         */
        Event recv_event;

        /**
         * Hidden constructor
         * @param operation_type send/recv event type
         * @param send_finish_time for send -- send operation ending time
         * @param fun_ptr for recv operation -- recv event handler
         * @param fun_arg for recv operation -- recv event handler argument
         */
        SendRecvTrackingMapValue(OperationType operation_type,
                                 AstraSim::timespec_t send_finish_time,
                                 void (*fun_ptr)(void *), void *fun_arg) noexcept :
                operation_type(operation_type),
                send_finish_time(send_finish_time),
                recv_event(fun_ptr, fun_arg) { }

    };
}

#endif //ANALYTICALBACKEND_SENDRECVTRACKINGMAPVALUE_H
