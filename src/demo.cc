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
#include <stdlib.h>
#include "statify/statify.h"

using statify::Event;
using statify::Statify;

int main(int argc, char* argv[]) {
  // Create an event writer instance
  // TODO(tdial): I like the name 'EventWriter' or 'EventLogger'; perhaps it
  //              should be the name of the class instead of Statify.
  Statify* event_writer = Statify::Create(NULL);

  // Print the address of the event writer.
  printf("event_writer = %p\n", event_writer);

  Event event;
  event.SetField("origin", "demo");
  event.SetField("useful", "true");

  int status = event_writer->LogEvent(event);
  printf("Statify::LogEvent() returned %d\n", status);

  // Delete the event writer.
  delete event_writer;
  return EXIT_SUCCESS;
}
