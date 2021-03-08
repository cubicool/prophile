# Prophile

Prophile is a small (single header / single source) C library with an
easy--**but robust**--API for profiling and measuring code execution time. It
has OS-level timing and querying support for Windows, as well as any environment
supporting the C function *gettimeofday*.

## Compiling / Integrating

**TODO**: Finish adding support to support including this project as a "git
submodule", and subsequently being used with the CMake *add_subdirectory*
command.

## Example

The typical developer will likely want to make use the "*standard*" API, which
involves creating a thread-local, opaque context object and passing it to each
of the corresponding functions.

Alternatively, Prophile can also be utilized by simply making use of a single
timing function: prophile_tick(). However, this ignores the other 95% of the
library, including the parts that make it fun and unique. :)

### (Standard Usage)

```
// TODO: ...
```

### (Questionable Usage)

```
// TODO: ...
```
