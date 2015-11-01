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

#ifndef INCLUDE_STATIFY_UTILITY_H_
#define INCLUDE_STATIFY_UTILITY_H_

#include <inttypes.h>
#include <stdlib.h>

namespace statify {

// Return the length of the string pointed to by 'str', not including
// the terminating NULL character, or return 'max', if the NULL terminator
// is not encountered within 'max' characters.
size_t string_length_or_max(const char *str, size_t max);

// Convert a 64 bit integer in the host byte order to network byte order.
int64_t htons64(int64_t host_int);

// Convert a 64 bit integer in the network byte order to host byte order.
int64_t ntohs64(int64_t net_int);

}  // namespace statify

#endif  // INCLUDE_STATIFY_UTILITY_H_
