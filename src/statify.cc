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

int Statify::LogEvent(const Event& event) {
  // TODO(tdial): Implement
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
