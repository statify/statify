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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "statify/log.h"

namespace {

// Long options
static struct option kLongOpts[] = {{"port", required_argument, 0, 'p'},
                                    {"backlog", required_argument, 0, 'b'},
                                    {"help", no_argument, 0, 'h'},
                                    {0, 0, 0, 0}};

// Short options string; must correspond to kLongOpts
static const char* kOptString = "p:b:";

// Default port number to use when listening for new TCP connections.
static const int kDefaultPortNumber = 9700;

// Default length of the listener's backlog queue.
static const int kDefaultBacklog = 10;

}  // namespace

namespace statify {

ProgramOptions::ProgramOptions()
    : port_(kDefaultPortNumber), backlog_(kDefaultBacklog), help_(false) {
}

void ProgramOptions::ParseCommandLine(int argc, char* argv[]) {
  for (;;) {
    int index = 0;
    int param = getopt_long(argc, argv, kOptString, kLongOpts, &index);
    if (param == -1) {
      break;
    }

    switch (param) {
      case 'p':
        set_port(atoi(optarg));
        break;

      case 'b':
        set_backlog(atoi(optarg));
        break;

      case 'h':
        set_help(true);
        break;

      default:
        Log::Write(Log::ABORT, "invalid argument: %c", param);
        break;
    }
  }
}

void ProgramOptions::DisplayHelp() {
  printf("TODO(tdial): DisplayHelp()\n");
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

int ProgramOptions::port() const {
  return port_;
}

void ProgramOptions::set_backlog(int backlog) {
  assert(backlog > 0);
  if (backlog < 1) {
    return;
  }
  backlog_ = backlog;
}

int ProgramOptions::backlog() const {
  return backlog_;
}

void ProgramOptions::set_help(bool help) {
  help_ = help;
}

bool ProgramOptions::help() const {
  return help_;
}

}  // namespace statify
