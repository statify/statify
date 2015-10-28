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

#include <event2/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "statify/log.h"
#include "statify/program_options.h"

using statify::Log;
using statify::ProgramOptions;

int main(int argc, char* argv[]) {
  // Extract program options
  ProgramOptions options(argc, argv);

  // Create event base
  struct event_base* evb = event_base_new();
  if (evb == NULL) {
    Log::Write(Log::ABORT, "failed to create event base");
    return EXIT_FAILURE;
  }

  Log::Write(Log::DEBUG, "starting event dispatching");

  // Dispatch events
  event_base_dispatch(evb);

  // Free the event base
  event_base_free(evb);

  return EXIT_SUCCESS;
}
