// Copyright 2015 The Statify Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License. See the AUTHORS file for names of
// contributors.

#include "statify/event_decoder.h"
#include <assert.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "statify/buffer.h"
#include "statify/event.h"
#include "statify/log.h"

namespace statify {

EventDecoder::EventDecoder()
    : state_(STATE_WAITING_HEADER), payload_size_(0), events_received_(0) {
  Log::Write(Log::DEBUG, "EventDecoder::EventDecoder()");
}

EventDecoder::~EventDecoder() {
  Log::Write(Log::DEBUG, "EventDecoder::~EventDecoder()");
}

void EventDecoder::OnData(evbuffer* input, evbuffer* output) {
  Log::Write(Log::DEBUG, "EventDecoder::OnData()");

  // While there is data available to consume
  for (;;) {
    // How much data is consumable?
    const size_t consumable_data = evbuffer_get_length(input);

    // TODO(tdial): Break out case statements into functions for clarity.
    switch (state_) {
      case STATE_WAITING_HEADER: {
        // To read the "header" we need at least the size of an uint32_t
        // since the length field is of that type. Just return. We'll
        // get called again when there's more data available.
        if (consumable_data < sizeof(uint32_t)) {
          return;
        }

        // Extract payload size, converting to host byte order.
        uint32_t tmp_payload_len = 0;
        evbuffer_remove(input, &tmp_payload_len, sizeof(uint32_t));
        payload_size_ = ntohl(tmp_payload_len);

        // Switch states: we're waiting for the body now.
        state_ = STATE_WAITING_BODY;
      } break;

      case STATE_WAITING_BODY: {
        // If we don't have all the data yet, just return.
        if (consumable_data < payload_size_) {
          return;
        }

        // Make the data contiguous; we need to copy it to a Buffer for
        // deserialization. TODO(tdial): Investigate whether we could
        // avoid copies here.
        unsigned char* data = evbuffer_pullup(input, payload_size_);
        Buffer tmp(reinterpret_cast<const char*>(data), payload_size_);

        // Record that we received another event.
        ++events_received_;

        // Invoke handler that processes the complete buffer.
        OnBuffer(tmp);

        // Drain the buffer, reset the payload size, and switch states
        // back to STATE_WAITING_HEADER to handle the next event.
        evbuffer_drain(input, payload_size_);
        payload_size_ = 0;
        state_ = STATE_WAITING_HEADER;
      } break;

      default: {
        Log::Write(Log::ABORT, "EventDecoder::OnData(): invalid decoder state");
        assert(false);
      } break;
    }
  }
}

void EventDecoder::OnBuffer(const Buffer& buffer) {
  Log::Write(Log::DEBUG, "EventDecoder::OnBuffer()");

  // Attempt deserializing an event from the buffer.
  Event event;
  const bool deserialized = Event::FromBuffer(buffer, &event);
  if (!deserialized) {
    Log::Write(Log::ERROR,
               "EventDecoder::OnBuffer(): Event::FromBuffer() returned false");
    return;
  }

  // We got an event: Log it
  Log::Write(Log::DEBUG, "Event timestamp: %lu", event.timestamp());

  // Iterate through the event's fields, and log them.
  Event::Iterator i = event.begin();
  Event::Iterator e = event.end();
  for (; i != e; ++i) {
    Log::Write(Log::DEBUG, "  Key: %s, Value: %s", i.Key().c_str(),
               i.Value().c_str());
  }
}

}  // namespace statify
