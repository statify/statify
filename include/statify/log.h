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

enum {
  LOG_ABORT  =  0,
  LOG_ERROR  =  1,
  LOG_WARN   =  2,
  LOG_INFO   =  3,
  LOG_DEBUG  =  4,
  LOG_XXXXX  =  5
};

void log(int level, const char* fmt, ...);

}  // namespace statify

#endif  // INCLUDE_STATIFY_LOG_H_
