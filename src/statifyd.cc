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
  // Log startup
  Log::Write(Log::INFO, "statifyd - Copyright (C) 2015 The Statify Authors");
  Log::Write(Log::INFO, "starting up");

  // Extract program options from command line
  ProgramOptions options;
  options.ParseCommandLine(argc, argv);

  // Log important startup parameters
  Log::Write(Log::INFO, "listener port: %d", options.port());
  Log::Write(Log::INFO, "listener size: %d", options.backlog());

  // Create event base
  struct event_base* evbase = event_base_new();
  if (evbase == NULL) {
    Log::Write(Log::ABORT, "failed to create event base");
    return EXIT_FAILURE;
  }

  // Create a TCP listener
  // struct evconnlistener* listener = NULL;
  // listener = evconnlistener_new_bind(evbase, accept_callback, 0

  Log::Write(Log::INFO, "startup complete");

  // Dispatch events
  event_base_dispatch(evbase);

  // Log that we're initiating shutdown
  Log::Write(Log::INFO, "shutting down");

  // Free the event base
  event_base_free(evbase);

  // Finally, log that we're done and exiting.
  Log::Write(Log::INFO, "shutdown complete");

  return EXIT_SUCCESS;
}
