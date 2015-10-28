#include <event2/event.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "statify/log.h"

using namespace statify;

int main(int argc, char* argv[]) {
  // Create event base
  struct event_base* evb = event_base_new();
  if (evb == NULL) {
    log(LOG_ABORT, "failed to create event base");
    return EXIT_FAILURE;
  }

  log(LOG_DEBUG, "starting event dispatching");

  // Dispatch events
  event_base_dispatch(evb);

  // Free the event base
  event_base_free(evb);

  return EXIT_SUCCESS;
}
