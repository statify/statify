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

#include "statify/portable.h"
#ifdef __linux__
  #include <byteswap.h>
  #include <endian.h>
#else
#endif

#define XOR_BYTE_SWAP(a, b) ((a)^=(b),(b)^=(a),(a)^=(b))

namespace statify {

int64_t ByteSwapInt64(int64_t value) {
#ifdef __linux__
  return bswap64(value);
#else
  unsigned char* buf = reinterpret_cast<unsigned char*>(&value);
  XOR_BYTE_SWAP(buf[0], buf[7]);
  XOR_BYTE_SWAP(buf[1], buf[6]);
  XOR_BYTE_SWAP(buf[2], buf[5]);
  XOR_BYTE_SWAP(buf[3], buf[4]);
  return value;
#endif
}

}  // namespace statify
