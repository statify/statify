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

#include <stdio.h>
#include "statify/buffer.h"
#include "statify/event.h"

using statify::Buffer;
using statify::Event;

int main(int argc, char *argv[]) {
  Event event(false);
  event.set_timestamp(717);
  event.SetField("last_name", "dial")
      .SetField("first_name", "tom")
      .SetField("age", 38);

  Buffer buf;
  const bool serialized = Event::EventToBuffer(event, &buf);
  printf("serialized? %s\n", serialized ? "yes" : "no");

  Event event2;
  const bool deserialized = Event::BufferToEvent(buf, &event2);
  printf("deserialized? %s\n", deserialized ? "yes" : "no");
  if (event.timestamp() == event2.timestamp()) {
    printf("timestamps matched\n");
  } else {
    printf("timestamps did no match\n");
  }

  printf("iterate through deserialized events:\n");
  Event::Iterator i = event2.begin();
  Event::Iterator e = event2.end();
  for (; i != e; ++i) {
    printf(" '%s' = '%s'\n", i.Key().c_str(), i.Value().c_str());
  }

  return 0;
}
