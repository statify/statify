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

#include "statify/event.h"
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "statify/buffer.h"
#include "statify/slice.h"
#include "statify/utility.h"

namespace {

static const char kStatifyVersion[] = {0x46, 0x44, 0x53, 0x00};

static const int kStatifyVersionSize = sizeof(kStatifyVersion);

// Read a NULL-terminated string from a slice, returning the string.
// The invocation advances the "window" in the slice past the NULL
// character, unless the string that was extracted was at the very
// end of the buffer with no NULL terminator.
std::string ReadStringFromSlice(statify::Slice *slice) {
  const size_t len =
      statify::string_length_or_max(slice->data(), slice->size());
  std::string str(slice->data(), len);
  slice->RemovePrefix(len);
  if (slice->size()) {
    slice->RemovePrefix(1);
  }
  return str;
}

}  // namespace

namespace statify {

Event::Event(bool init_with_now) : timestamp_(init_with_now ? Now() : 0) {
}

Event &Event::set_timestamp(int64_t epoch_nanos) {
  timestamp_ = epoch_nanos;
  return *this;
}

int64_t Event::timestamp() const {
  return timestamp_;
}

bool Event::HasField(const std::string &field_name) const {
  const data_type::const_iterator i = data_.find(field_name);
  return (i != data_.end());
}

const std::string &Event::GetField(const std::string &field_name) const {
  static const std::string empty("");
  data_type::const_iterator i = data_.find(field_name);
  return ((i != data_.end()) ? i->second : empty);
}

int64_t Event::Now() {
  const size_t kBillion = 1000000000;
  struct timespec ts = {0};
  clock_gettime(CLOCK_REALTIME, &ts);
  return (ts.tv_sec * kBillion) + ts.tv_nsec;
}

bool Event::EventToBuffer(const Event &event, Buffer *buffer) {
  assert(buffer != NULL);
  if (buffer == NULL) {
    return false;
  }

  // Append the version header to the message.
  buffer->Append(kStatifyVersion, sizeof(kStatifyVersion));

  // Append the time stamp.
  // TODO(tdial): Deal with endianness.
  buffer->Append((const char *)(&(event.timestamp_)), sizeof(event.timestamp_));

  // Append all key/value pairs. We append all strings like this:
  //   <key0>\0<value0>\0<key1>\0<value1>\0\0
  // The entire message is terminated by a null character.
  data_type::const_iterator i = event.data_.begin();
  const data_type::const_iterator e = event.data_.end();
  for (; i != e; ++i) {
    buffer->Append(i->first.c_str(), i->first.size() + 1);
    buffer->Append(i->second.c_str(), i->second.size() + 1);
  }

  buffer->AppendNull();

  return true;
}

bool Event::BufferToEvent(const Buffer &buffer, Event *event) {
  assert(event != NULL);
  if (event == NULL) {
    return false;
  }

  // Create a slice, which may be considered to be a sliding window into
  // the buffer that we can advance cheaply without any copying.
  Slice slice(buffer);

  // First, ensure that we have at least enough data in the message to
  // hold a valid header, which contains a four-byte version identifier,
  // followed by an eight-byte timestamp.
  const size_t kMinMsgSize = sizeof(kStatifyVersionSize) + sizeof(int64_t);
  if (slice.size() < kMinMsgSize) {
    return false;
  }

  // Ensure that the version header is valid.
  if (0 != memcmp(kStatifyVersion, slice.data(), kStatifyVersionSize)) {
    return false;
  }

  // Advance slice past the version header.
  slice.RemovePrefix(kStatifyVersionSize);

  // Copy time stamp directly into the user's event
  // TODO(tdial): Deal with endianness.
  memcpy(&(event->timestamp_), slice.data(), sizeof(int64_t));

  // Advance past the timestamp portion of the message.
  slice.RemovePrefix(sizeof(int64_t));

  // Read key/values from the buffer. Format is:
  //   <key0>\0<value0>\0<key1>\0<value1>\0\0
  // The entire message is terminated by a NULL character.
  while (slice.size() > 0) {
    std::string key = ReadStringFromSlice(&slice);
    std::string val = ReadStringFromSlice(&slice);
    if (key.size()) {
      event->data_.insert(data_type::value_type(key, val));
    }
  }

  return true;
}

Event::Iterator Event::begin() const {
  return Iterator(data_.begin());
}

Event::Iterator Event::end() const {
  return Iterator(data_.end());
}

}  // namespace statify
