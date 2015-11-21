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

#include "statify/client.h"
#include <assert.h>

namespace {

const int kMinPort = 0;
const int kMaxPort = 65535;

}  // namespace

namespace statify {

Client::Options::Options() {
}

Client::Options& Client::Options::set_host(const std::string& host_or_ip) {
  host_ = host_or_ip;
  return *this;
}

const char* Client::Options::host() const {
  return host_.c_str();
}

Client::Options& Client::Options::set_port(int port) {
  if ((port >= kMinPort) && (port <= kMaxPort)) {
    port_ = port;
  }
  return *this;
}

int Client::Options::port() const {
  return port_;
}

Client::~Client() {
}

Client* Client::Create(const Options* options, int* opt_error) {
  // TODO(tdial): Implement
  return NULL;
}

int Client::SendEvent(const Event& event) {
  // TODO(tdial): Implement
  return -1;
}

void* Client::ThreadProcedure(void* thread_arg) {
  assert(thread_arg != NULL);
  Client* self = reinterpret_cast<Client*>(thread_arg);
  self->ThreadProcedure();
  return NULL;
}

void ThreadProcedure() {
  // TODO(tdial): Implement
}

}  // namespace statify
