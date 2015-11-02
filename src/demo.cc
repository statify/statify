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
  // Initialize Statify
  int status = Statify::Initialize();
  printf("Statify::Initialize() returned %d\n", status);

  // Prepare an event for logging
  Event event;
  event.SetField("author", "dial");
  event.SetField("origin", "demo");
  event.SetField("useful", "true");

  // Log the event
  status = Statify::LogEvent(event);
  printf("Statify::LogEvent() returned %d\n", status);

  // Shutdown Statify
  status = Statify::Shutdown();
  printf("Statify::Shutdown() returned %d\n", status);

  return EXIT_SUCCESS;
}
