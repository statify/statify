# Statify

Statify is a statistics gathering system consisting of a client library
and server that enables programs to reliably log events for collection
and analysis. Clients interested in logging events to a Statify server
link to a client library that utilizes a custom TCP protocol to
communicate with the server.

The role of the Statify server is to collect, filter, and store
statistics gathered from clients: several backends will be supported in
the initial version, including a simple logging backend as well as a
backend that indexes directly into ElasticSearch.

The Statify server is written in C++, and utilizes the libevent event-
driven framework in order to efficiently handle potentiall thousands of
inbound client connections. The Statify client library will expose C
and C++ APIs in the initial version, with other languages to follow
(Plans are to support Java, Go, and Python.) Those wishing to port the
client library to other languages should contact the authors for
assistance.

## Statify Events

The fundamental unit of work within the Statify ecosystem is the
"event." An Event is simply a time-stamped dictionary of arbitrary
name/value pairs, with the restriction that names that begin with
leading underscores are reserved for use by the statify protocol.

The client library will provide easy-to-use APIs for constructing
events and dispatching them to a Statify server, while avoiding
blocking on synchronous network I/O. The client library will be highly
portable, robust, and will support automatic reconnect on network
failure, as well as being thread-safe and fork-safe.

## Current Status

Statify is currently in alpha.

