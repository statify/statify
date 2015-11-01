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

#ifndef INCLUDE_STATIFY_STATIFY_H_
#define INCLUDE_STATIFY_STATIFY_H_

#include <assert.h>
#include <string>
#include "statify/default_port_number.h"
#include "statify/event.h"

namespace statify {

class Statify {
 public:
  ~Statify();

  // Options structure used to specify alternate host and port.
  // If NULL is passed to Statify::Create(), localhost and port 9700 are used.
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

  // Create an instance of the Statify event logger.
  static Statify* Create(const Options* options = NULL);

 private:
  Statify();
  Statify(const Statify& no_copy);
  Statify& operator=(const Statify& no_assign);
};

}  // namespace statify

#endif  // INCLUDE_STATIFY_STATIFY_H_
