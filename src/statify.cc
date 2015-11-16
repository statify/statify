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

#include "statify/statify.h"
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include "statify/buffer.h"
#include "statify/event.h"
#include "statify/default_port_number.h"
#include "statify/slice.h"

namespace {

// TODO(tdial): The max payload for a single message, in bytes.
// This must always be a number that can be represented by uint32_t
const size_t kMaxEventPayloadSize = 16777216;  // 16MB

// Append an event to the buffer.
// Returns number of bytes added, or zero if the event could not be serialized
// to the buffer. The only reason for failure would be that the event, once
// serialized, exceeded the maximum message length. In the event that this
// occurs, the buffer is resized to its original state.
size_t AppendEventToBuffer(const statify::Event& event, statify::Buffer* buf) {
  // Begin by saving the length of the buffer, before appending the event.
  // This will be used later to calculate the true size of the payload as
  // well as to return the number of bytes appended to the buffer.
  const size_t start_length = buf->Size();

  // Next, append the length value to the buffer. We won't know the size
  // of the payload until we serialize it, so push zero for now; we'll fix
  // it up afterward. This avoids calculating the length twice / minimizes
  // the data copying that would otherwise be needed to make the length
  // and and payload to be stored contiguously.
  uint32_t payload_tmp = 0;
  buf->Append((const char*)&payload_tmp, sizeof(payload_tmp));

  // TODO(tdial): Event::ToBuffer() should return the number of bytes
  // that were *added* to the buffer.
  bool status = statify::Event::ToBuffer(event, buf);
  if (!status) {
    return 0;
  }

  // Get the final size of the buffer.
  const size_t end_length = buf->Size();

  // Enforce the maximum buffer length.
  const size_t bytes_added = end_length - start_length;
  if (bytes_added > kMaxEventPayloadSize) {
    buf->Resize(start_length);
    return 0;
  }

  // Calculate payload size, and prepare for poking back into the buffer.
  uint32_t actual_payload_size =
      htonl((uint32_t)bytes_added - sizeof(uint32_t));

  // Poke the actual payload size back into the buffer.
  char* data = buf->Data();
  data += start_length;
  *(reinterpret_cast<uint32_t*>(data)) = actual_payload_size;

  // Return the number added.
  return bytes_added;
}

// Return 'true' if we may retry a write
inline bool RetryableSocketError(int error) {
  return ((error == EINTR) || (error == EAGAIN));
}

}  // namespace

namespace statify {

Statify::~Statify() {
}

Statify::Options::Options() : host_("127.0.0.1"), port_(kDefaultPortNumber) {
}

Statify::Options& Statify::Options::set_host(const std::string& host_or_ip) {
  host_ = host_or_ip;
  return *this;
}

const char* Statify::Options::host() const {
  return host_.c_str();
}

Statify::Options& Statify::Options::set_port(int port) {
  const int kMinPort = 1;
  const int kMaxPort = 65535;
  assert(port >= kMinPort);
  assert(port <= kMaxPort);
  if ((port >= kMinPort) && (port <= kMaxPort)) {
    port_ = port;
  }
  return *this;
}

int Statify::Options::port() const {
  return port_;
}

int Statify::Initialize(const Options* options) {
  // TODO(tdial): Implement
  return 0;
}

// Naive implementation: connects each time with a new socket!
int Statify::LogEvent(const Event& event) {
  // Serialize the event to a buffer, for transmission on the network.
  Buffer buffer;
  const size_t num_appended = AppendEventToBuffer(event, &buffer);
  if (num_appended == 0) {
    return EINVAL;
  }

  // Create a TCP socket
  // TODO(tdial): Create an RAII wrapper to close the socket.
  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    return errno;
  }

  // Construct an address
  // TODO(tdial): Hard coded to localhost/9700
  const char* kLocalHostName = "127.0.0.1";
  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(kDefaultPortNumber);
  addr.sin_addr.s_addr = inet_addr(kLocalHostName);

  // Attempt to connect to the server
  int status =
      connect(sock, (const struct sockaddr*)&addr, sizeof(struct sockaddr));
  if (status != 0) {
    close(sock);
    return errno;
  }

// On Linux, we can specify that we don't want failed writes to cause a
// SIGPIPE by passing the MSG_NOSIGNAL flag. This is not available on
// all platforms, including Mac OSX. We'll address this in the future
// by making the thread that handles the send block the signal. Also,
// we should allow certain errors (EAGAIN)
// TODO(tdial): blocks signals and handle allowable errors.
#ifdef __linux__
  const int kSendFlags = MSG_NOSIGNAL;
#else
  const int kSendFlags = 0;
#endif

  // Write the event to the socket.
  Slice slice(buffer);
  while (!slice.IsEmpty()) {
    // Send as many bytes as we can. Don't generate signals.
    int sent = send(sock, slice.data(), slice.size(), kSendFlags);
    if (sent < 0) {
      if (RetryableSocketError(errno)) {
        continue;
      }
      close(sock);
      return errno;
    }
    // Advance our "window" into the buffer.
    slice.RemovePrefix(sent);
  }

  // Finally, close the socket
  close(sock);

  return 0;
}

int Statify::Shutdown() {
  // TODO(tdial): Implement
  return 0;
}

Statify::Statify() {
  // TODO(tdial): Implement
}

}  // namespace statify
