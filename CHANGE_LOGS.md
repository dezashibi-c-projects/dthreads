## v0.9.71-stable

**Release Date: 2024-08-22**

- rename dthread_new_config to dthread_init_thread for better readability
- improve macros by removing redundant checking
- fix problem on apple devices
- tested on apple, linux, windows, works as expected

=======

## v0.9.52-stable

**Updated on 2024-08-16**

Release Date: 2024-08-13

- improve integer type portability
- improve documentations
- improve examples
- improve macro usages
- update API: remove DThreadConfig and bundle it's data to DThread tend to more simplified approach
- add DTHREAD_API for supporting dynamic and static linking
- add thread safe random number generator
- add example for trylock
- add macro for casting thread result
- add Makefile target to run all the compiled examples
- add semaphore
- add semaphore example
- add barrier
- add barrier example
- add documentations
- prepare makefile
- fix windows bug for trylock
- fix the bug with handling returned results from thread on windows
- fix bugs in windows and linux implementations about double reference
- license clarification

=======

## v0.1.0

**Release Date: 2024-08-12**

- Working example
- Fully modularized and cross platform test

=======

## v0.0.1

**Release Date: 2024-08-12**

- initialize the repo
