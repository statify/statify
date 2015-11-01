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

#ifndef INCLUDE_STATIFY_PROGRAM_OPTIONS_H_
#define INCLUDE_STATIFY_PROGRAM_OPTIONS_H_

namespace statify {

class ProgramOptions {
 public:
  // Construct with reasonable defaults.
  ProgramOptions();

  // Parse options from the command-line argument list.
  void ParseCommandLine(int argc, char* argv[]);

  // Print command-line options.
  void DisplayHelp();

  // Set/get the listener backlog queue length.
  void set_backlog(int backlog);
  int backlog() const;

  // Set/get the help option.
  void set_help(bool help);
  bool help() const;

  // Set the maximum message size in bytes that the server will allow.
  // The server will terminate connections that send larger messages.
  void set_max_message_size(int max_size);
  int max_message_size();

  // Set/get the TCP port number to listen on for new connections.
  void set_port(int port);
  int port() const;

 private:
  int backlog_;
  bool help_;
  int max_message_size_;
  int port_;
};

}  // namespace statify

#endif  // INCLUDE_STATIFY_PROGRAM_OPTIONS_H_
