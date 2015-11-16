This directory contains an experiment testing the behavior of threads
and their interaction with fork.

Only one thread survives a call to fork, and thus the background thread
created by the statify library will have to be recreated AFTER the fork.

