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

#ifndef INCLUDE_STATIFY_CLIENT_H_
#define INCLUDE_STATIFY_CLIENT_H_

#include <pthread.h>
#include <string>
#include "statify/event.h"

namespace statify {

class Client {
 public:
  // Options: used to configure instances of the client.
  class Options {
   public:
    Options();

    // Set the host name string.
    Options& set_host(const std::string& host_or_ip);

    // Return the host name string.
    const char* host() const;

    // Set the port number.
    Options& set_port(int port);

    // Return the port number.
    int port() const;

   private:
    std::string host_;
    int port_;
  };

  // Destructor
  ~Client();

  // Create a client with the specified options.
  // The caller may pass NULL for 'options'. In this event, the client will
  // attempt to connect to the local host on the default port.
  // Returns valid object on success. Returns NULL on failure. Populates
  // (optional) integer with reason code in the event of an error.
  static Client* Create(const Options* options, int* opt_error);

  // Send an event to the collector daemon.
  int SendEvent(const Event& event);

 private:
  // Copy and assignment are explicitly disallowed.
  Client(const Client& no_copy);
  Client& operator=(const Client& no_assign);

  // Private thread procedure; turns around an invokes the member, below.
  static void* ThreadProcedure(void* arg);

  // Invoked by the static version of the same name. Performs I/O.
  void ThreadProcedure();

  // Synchronization primitives used to manage event queue.
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
};

}  // namespace statify

#endif  // INCLUDE_STATIFY_CLIENT_H_
