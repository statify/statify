# Copyright 2015 The Statify Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. See the AUTHORS file for names of
# contributors.


# Uncomment exactly one of the lines labelled (A), (B), and (C) below
# to switch between compilation modes.

# A: Production use (full optimizations)
#OPT ?= -O3 -DNDEBUG

# B: Debug mode, with full line-level debugging symbols
OPT ?= -g2

# C: Profiling mode: optimizations, but w/debugging symbols
#OPT ?= -O3 -g2 -DNDEBUG

# TODO(tdial): Use for install target
PREFIX ?= /usr/local

# Warning Flags
WARNINGFLAGS = -Wall -Werror

# Run a script to generate SOURCES and set platform-specific options.
$(shell CXX="$(CXX)" ./build_config build_config.mk)

# Include the file generated by build_config
include build_config.mk

CXXFLAGS += -I. -I./include $(OPT) $(WARNINGFLAGS)

DEMO_OBJS = $(DEMO_SOURCES:.cc=.o)
STATIFYD_OBJS = $(STATIFYD_SOURCES:.cc=.o)
STATIFY_OBJS = $(STATIFY_SOURCES:.cc=.o)
TESTS = 

# Targets
all: demo libstatify.a statifyd

.PHONY:
check: $(TESTS)
	for t in $(TESTS); do echo "** Running $$t"; ./$$t || exit 1; done

.PHONY:
clean:
	-rm -f */*.o build_config.mk *.a demo libstatify.a statifyd

.PHONY:
count:
	wc -l $(CPPLINT_SOURCES)

# Demo program. It only links to libstatify.a and pthreads
demo: $(DEMO_OBJS) libstatify.a
	$(CXX) $(CXXFLAGS) -o demo $(DEMO_OBJS) $(DEMO_LIBRARIES) libstatify.a

libstatify.a: $(STATIFY_OBJS)
	ar rcs libstatify.a $(STATIFY_OBJS)

# Statify daemon
statifyd: $(STATIFYD_OBJS) libstatify.a
	$(CXX) $(CXXFLAGS) $(ALT_LIBEVENT_LIBRARY) -o statifyd $(STATIFYD_OBJS) $(LIBRARIES) libstatify.a

# Lint check
.PHONY:
lint:
	$(LINT_TOOL) $(CPPLINT_SOURCES)

# Clean
.PHONY:
neat: clean
	-rm -f *~ .*~ */*~ ./include/statify/*~ ./src/*~

# Reformat source code according to project standards
.PHONY:
reformat:
	clang-format -i $(CPPLINT_SOURCES)

# Tests

# Suffix Rules
.cc.o:
	$(CXX) $(CXXFLAGS) $(ALT_LIBEVENT_INCLUDE) -c $< -o $@

