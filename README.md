# Prophile

Prophile is a small (single header / single source) C library using an
easy--**but robust**--API for profiling and measuring code execution time. It
has OS-level timing and querying support for Windows, as well as any environment
supporting the C function *clock_gettime*. Alternatively, it can be configured
to use the [RDTSC](https://en.wikipedia.org/wiki/Time_Stamp_Counter) assembly
instruction, if supported.

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

## TODO

- [ ] Document and define the scope and intentions of the library.
- [ ] Should there be a PAUSE feature?
- [ ] Introduce some kind of mutex mechanism.
- [ ] Add additional LD_PRELOAD style injection support.
- [ ] Determine when \_\_declspec is needed.
- [ ] Use [STB](https://github.com/nothings/stb) if internal data management
  gets too complicated.
