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

#ifndef INCLUDE_STATIFY_LOG_H_
#define INCLUDE_STATIFY_LOG_H_

#include <stdarg.h>

namespace statify {

class Log {
 public:
  enum Level { ABORT = 0, ERROR = 1, WARN = 2, INFO = 3, DEBUG = 4 };

  static void Write(int level, const char* fmt, ...);
};

}  // namespace statify

#endif  // INCLUDE_STATIFY_LOG_H_
