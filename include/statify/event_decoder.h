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

#ifndef INCLUDE_STATIFY_EVENT_DECODER_H_
#define INCLUDE_STATIFY_EVENT_DECODER_H_

#include <stdlib.h>

// Forward declaration for libevent type
struct evbuffer;

namespace statify {

class Buffer;

// The EventDecoder class is responsible for decoding Event objects from
// the "wire." The event framework instanstiates an EventDecoder when a
// new client connection is accepted. The system supports persistent
// connections, so a single instance of an event decoder may decode many
// events from the TCP channel over its lifetime.
class EventDecoder {
 public:
  EventDecoder();
  ~EventDecoder();

  // Invoked when there is I/O to perform on the underlying socket.
  void OnData(evbuffer* input, evbuffer* output);

  // Return diagnostic count of events received.
  size_t events_received() const;

 private:
  EventDecoder(const EventDecoder& no_copy);
  EventDecoder& operator=(const EventDecoder& no_assign);

  // Invoked when a complete event buffer is deserialized.
  void OnBuffer(const Buffer& buffer);

  enum {
    STATE_WAITING_HEADER = 0,  // Waiting for the length header
    STATE_WAITING_BODY = 1     // Waiting for body data
  };

  int state_;
  size_t payload_size_;
  size_t events_received_;
};

}  // namespace statify

#endif  // INCLUDE_STATIFY_EVENT_DECODER_H_
