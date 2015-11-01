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

#include "statify/utility.h"
#include <byteswap.h>
#include <endian.h>

namespace statify {

size_t string_length_or_max(const char *str, size_t max) {
  for (size_t i = 0; i < max; ++i) {
    if (str[i] == '\0') {
      return i;
    }
  }
  return max;
}

int64_t htons64(int64_t host_int) {
#ifdef LITTLE_ENDIAN
  return bswap_64(host_int);
#else
  return host_int;
#endif
}

int64_t ntohs64(int64_t net_int) {
#ifdef LITTLE_ENDIAN
  return bswap_64(net_int);
#else
  return net_int;
#endif
}

}  // namespace statify
