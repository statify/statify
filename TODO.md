# Project TODO List

The bulleted items below are intended to be kept in priority order from
greatest priority to least priority.

* Pending
  * Server should timeout on connections that hang for a long time
    * This implies working reconnect logic for clients
  * Handle fork() in client library
  * Add C language interface
  * Perhaps handlers should be loadable modules (shared libs)
  * Move non-public headers to source directory
  * Add an "install" target to Makefile
  * Server should support logging levels
  * Server should support writing logs into JSON for easy logstash integration.

* Completed
  * Implement basics of a client library
    * Ability to send a message to a localhost Statify daemon
