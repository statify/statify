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
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include "statify/log.h"

namespace statify {

EventDecoder::EventDecoder() {
  Log::Write(Log::DEBUG, "EventDecoder::EventDecoder()");
}

EventDecoder::~EventDecoder() {
  Log::Write(Log::DEBUG, "EventDecoder::~EventDecoder()");
}

void EventDecoder::OnData(struct evbuffer* input, struct evbuffer* output) {
  Log::Write(Log::DEBUG, "EventDecoder::OnData()");
}

}  // namespace statify
