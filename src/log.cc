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

#include "statify/log.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

namespace {

static const char* LevelToString(int level) {
  using statify::Log;

  const int kInvalidLevelIndex = 5;

  // Potentially nudge level into range
  if ((level < Log::ABORT) || (level > Log::DEBUG)) {
    level = kInvalidLevelIndex;
  }

  static const char* kLevels[] = {"ABORT", "ERROR", "WARN ",
                                  "INFO ", "DEBUG", "XXXXX"};

  return kLevels[level];
}

}  // namespace

namespace statify {

void Log::Write(int level, const char* fmt, ...) {
  // Get current time stamp
  struct timeval tv = {0};
  gettimeofday(&tv, NULL);

  // Get the time parts for the time stamp
  struct tm time_parts = {0};
  memset(&time_parts, 0, sizeof(time_parts));
  gmtime_r(&(tv.tv_sec), &time_parts);

  // Convert microseconds from tv to milliseconds
  const int milliseconds = tv.tv_usec / 1000;

  // Print timestamp and log level
  fprintf(stderr, "%04d-%02d-%02d %02d:%02d:%02d:%04d %s | ",
          time_parts.tm_year + 1900, time_parts.tm_mon + 1, time_parts.tm_mday,
          time_parts.tm_hour, time_parts.tm_min, time_parts.tm_sec,
          milliseconds, LevelToString(level));

  // Print user message
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);

  // Print the end-of-line
  fprintf(stderr, "\n");

  fflush(stderr);

  // Finally, if the log level was LOG_ABORT, then abort
  if (level == Log::ABORT) {
    abort();
  }
}

}  // namespace statify
