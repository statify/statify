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
#include <stdlib.h>

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
  if (!initialized_) {
    return;
  }

  // TODO(tdial): Signal thread to shut down.

  // Join on the thread.
  // TODO(tdial): Pay attention to the status
  int status = pthread_join(thread_, NULL);
  (void)status;

  // Release sync primitives.
  pthread_cond_destroy(&cond_);
  pthread_mutex_destroy(&mutex_);
}

Client* Client::Create(const Options* options, int* opt_error) {
  Client* client = new Client();
  int status = client->Initialize(options);
  if (status != 0) {
    if (opt_error) {
      *opt_error = status;
    }
    delete client;
    return NULL;
  }
  return client;
}

int Client::SendEvent(const Event& event) {
  // TODO(tdial): Implement
  return -1;
}

Client::Client()
    : thread_(0),
      thread_run_state_(kThreadRunStateNotStarted),
      thread_initialization_error_(0),
      initialized_(false) {
}

int Client::Initialize(const Options* options) {
  // TODO(tdial): access options
  (void)options;

  // Initialize synchronization primitives
  int status = pthread_mutex_init(&mutex_, NULL);
  if (status != 0) {
    return status;
  }

  status = pthread_cond_init(&cond_, NULL);
  if (!status) {
    pthread_mutex_destroy(&mutex_);
    return status;
  }

  // Create the worker thread
  status = pthread_create(&thread_, NULL, ThreadProcedure, this);
  if (status != 0) {
    // TODO(tdial): Need a ScopeGuard class.
    pthread_cond_destroy(&cond_);
    pthread_mutex_destroy(&mutex_);
    return status;
  }

  // Wait until the thread has completed initialization. There are
  // two outcomes possible: if the state is kThreadRunStateStarted,
  // this means that we've completed all initialization activities
  // in the thread and may return success. If, however, the state
  // is kThreadRunStateFailedToStart, then we will join on the thread
  // and proceed with cleaning up the objects acquired above.

  // First, initialize a copy of the state. The condition variable
  // protects the member 'thread_run_state_.' We'll copy that value
  // to our local copy under protection of the mutex.
  int state = kThreadRunStateNotStarted;
  int error = 0;

  // Wait for state change.
  pthread_mutex_lock(&mutex_);
  while (thread_run_state_ == kThreadRunStateNotStarted) {
    pthread_cond_wait(&cond_, &mutex_);
    if (thread_run_state_ != kThreadRunStateNotStarted) {
      state = thread_run_state_;
      error = thread_initialization_error_;
      break;
    }
  }
  pthread_mutex_unlock(&mutex_);

  // Did the worker thread compelte initialization successfully?
  if (state == kThreadRunStateStarted) {
    // Yes, set 'initialized_' flag and return success.
    initialized_ = true;
    return 0;
  } else if (state == kThreadRunStateFailedToStart) {
    // No, we failed to start.
    // TODO(tdial): We should do something about a failed join.
    int status = pthread_join(thread_, NULL);
    (void)status;
    pthread_cond_destroy(&cond_);
    pthread_mutex_destroy(&mutex_);
    thread_ = 0;
    return error;
  } else {
    // Some other state. This reflects a bug.
    assert(!"thread_run_state_ was in unexpected state");
    abort();
  }

  // Unreachable
  return -1;
}

void* Client::ThreadProcedure(void* thread_arg) {
  assert(thread_arg != NULL);
  Client* self = reinterpret_cast<Client*>(thread_arg);
  self->ThreadProcedure();
  return NULL;
}

void Client::ThreadProcedure() {
  // TODO(tdial): Implement
}

}  // namespace statify
