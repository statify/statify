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

#include "statify/program_options.h"
#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include "statify/log.h"

namespace {

static struct option kLongOpts[] = {{"port", required_argument, 0, 'p'},
                                    {0, 0, 0, 0}};

static const char* kOptString = "p:";

}  // namespace

namespace statify {

ProgramOptions::ProgramOptions(int argc, char* argv[]) {
  for (;;) {
    int index = 0;
    int param = getopt_long(argc, argv, kOptString, kLongOpts, &index);
    if (param == -1) {
      break;
    }

    switch (param) {
      case 'p': {
        set_port(atoi(optarg));
      } break;

      default: { Log::Write(Log::ABORT, "invalid argument: %c", param); } break;
    }
  }
}

void ProgramOptions::set_port(int port) {
  assert(port >= 0);
  assert(port <= 65535);
  if ((port < 0) || (port > 65535)) {
    Log::Write(Log::ABORT, "invalid port number specified: %d", port);
    return;
  }
  port_ = port;
}

int ProgramOptions::port() const { return port_; }

}  // namespace statify
