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
#include <assert.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <event2/event.h>
#include <event2/listener.h>
#include <inttypes.h>
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

void buffer_read_callback(struct bufferevent* bev, void* ctx) {
  Log::Write(Log::INFO, "buffer_read_callback()");

  // TODO(tdial): Can bev or ctx be NULL?
  assert(bev != NULL);
  assert(ctx != NULL);

  // Access the input
  // TODO(tdial): Can this function return NULL?
  struct evbuffer* in = bufferevent_get_input(bev);
  assert(in != NULL);

  // Access the output
  // TODO(tdial): Can this function return NULL?
  struct evbuffer* out = bufferevent_get_output(bev);
  assert(out != NULL);

  // Write out whatever we read
  evbuffer_add_buffer(out, in);
}

void buffer_write_callback(struct bufferevent* bev, void* ctx) {
  // TODO(tdial): Implement
  Log::Write(Log::INFO, "buffer_write_callback()");
}

void buffer_event_callback(struct bufferevent* bev, int16_t events, void* ctx) {
  Log::Write(Log::INFO, "buffer_event_callback()");

  if (events & BEV_EVENT_ERROR) {
    Log::Write(Log::ERROR, "buffer_event_callback(): BEV_EVENT_ERROR");
  }

  if (events & BEV_EVENT_EOF) {
    Log::Write(Log::INFO, "buffer_event_callback(): BEV_EVENT_EOF");
    bufferevent_free(bev);
  }
}

// Callback that is registered to handle new connection attempts.
void accept_callback(struct evconnlistener* listener, evutil_socket_t sock,
                     struct sockaddr* addr, int, void* arg) {
  // TODO(tdial): Can listener ever be NULL?
  assert(listener != NULL);

  // TODO(tdial): Can socket ever be invalid?
  assert(sock >= 0);

  // TODO(tdial): Can a valid listener ever provide an invalid base?
  struct event_base* base = evconnlistener_get_base(listener);
  assert(base != NULL);

  // Construct buffer event object
  struct bufferevent* bev(NULL);
  bev = bufferevent_socket_new(base, sock, BEV_OPT_CLOSE_ON_FREE);

  // TODO(tdial): In what circumstances may bufferevent_socket_new() fail?
  assert(bev != NULL);

  // Configure event callbacks
  bufferevent_setcb(bev, buffer_read_callback, buffer_write_callback,
                    buffer_event_callback, NULL);

  bufferevent_enable(bev, EV_READ | EV_WRITE);
}

// Callback that is registered to handle errors occurring during accept.
void accept_error_callback(struct evconnlistener* listener, void* arg) {
  (void)listener;
  (void)arg;

  Log::Write(Log::ERROR, "failed to accept connection");
}

}  // namespace

int main(int argc, char* argv[]) {
  // Extract program options from command line
  ProgramOptions options;
  options.ParseCommandLine(argc, argv);
  if (options.help()) {
    options.DisplayHelp();
    return EXIT_SUCCESS;
  }

  // Log startup
  Log::Write(Log::INFO, "statifyd - Copyright (C) 2015 The Statify Authors");
  Log::Write(Log::INFO, "starting up");

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
