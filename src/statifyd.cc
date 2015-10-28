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

#include <arpa/inet.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "statify/log.h"
#include "statify/program_options.h"

using statify::Log;
using statify::ProgramOptions;

namespace {

// Callback that is registered to handle new connection attempts.
void accept_callback(struct evconnlistener* listener, evutil_socket_t sock,
                     struct sockaddr* addr, int, void* arg) {
  // Reference arguments
  (void)listener;
  (void)sock;
  (void)addr;
  (void)arg;

  Log::Write(Log::INFO, "accepted connection");
  close(sock);
  Log::Write(Log::INFO, "closed connection");
}

// Callback that is registered to handle errors occurring during accept.
void accept_error_callback(struct evconnlistener* listener, void* arg) {
  (void)listener;
  (void)arg;

  Log::Write(Log::ERROR, "failed to accept connection");
}

}  // namespace

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

  // Construct wildcard address for listening on the chosen port.
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = htonl(0);
  address.sin_port = htons(options.port());

  // Create a TCP listener
  struct evconnlistener* listener = NULL;
  listener = evconnlistener_new_bind(
      evbase, accept_callback, 0, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE,
      options.backlog(), reinterpret_cast<struct sockaddr*>(&address),
      sizeof(address));
  if (listener == NULL) {
    Log::Write(Log::ABORT, "failed to establish listener");
  }

  // Configure a callback function to invoke on errors
  evconnlistener_set_error_cb(listener, accept_error_callback);

  Log::Write(Log::INFO, "startup complete");

  // Dispatch events: This runs the event loop.
  event_base_dispatch(evbase);

  evconnlistener_free(listener);

  // Log that we're initiating shutdown
  Log::Write(Log::INFO, "shutting down");

  // Free the event base
  event_base_free(evbase);

  // Finally, log that we're done and exiting.
  Log::Write(Log::INFO, "shutdown complete");

  return EXIT_SUCCESS;
}
