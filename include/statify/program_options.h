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
  ProgramOptions(int argc, char* argv[]);
  // Default copy, assignment, and destructor OK.

  void set_port(int port);
  int port() const;

 private:
  int port_;
};

}  // namespace statify

#endif  // INCLUDE_STATIFY_PROGRAM_OPTIONS_H_
