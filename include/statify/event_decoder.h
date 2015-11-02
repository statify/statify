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

#ifndef STATIFY_EVENT_DECODER_H_
#define STATIFY_EVENT_DECODER_H_

namespace statify {

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
  void OnData(struct evbuffer* input, struct evbuffer* output);

 private:
  EventDecoder(const EventDecoder& no_copy);
  EventDecoder& operator=(const EventDecoder& no_assign);
};

}  // namespace statify

#endif  // STATIFY_EVENT_DECODER_H_
