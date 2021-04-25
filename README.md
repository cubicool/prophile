# Prophile

Prophile is a small (single header / single source) C library using an
easy--**but robust**--API for profiling and measuring code execution time. It
has OS-level timing and querying support for Windows, as well as any environment
supporting the C function *clock_gettime*. Alternatively, it can be configured
to use the [RDTSC](https://en.wikipedia.org/wiki/Time_Stamp_Counter) assembly
instruction, if supported.

## Compiling / Integrating

**TODO**: Finish adding support to facilitate including this project as a "git
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

Standard usage will generally look something like the following (**NOTE:** this
doesn't demonstrate handling the returned duration from *prophile_stop*, using
*callbacks*, and a whole host of other things):

```
prophile_t p = prophile_create(PROPHILE_USEC);

prophile_start(p, "foo"); {
    // ...

    prophile_start(p, "bar"); {
        // ...

        prophile_start(p, "baz0");
        prophile_stop(p);

        // ...

        prophile_start(p, "baz1");
        prophile_stop(p);
    }

    prophile_stop(p);
}

prophile_stop(p);

prophile_destroy(p);
```

### (Questionable Usage)

For those who insist, you could simply calculate the durations on your own. But
then, why are you using a library?

```
prophile_tick_t start = prophile_tick(PROPHILE_NSEC);
// ...
prophile_tick_t stop = prophile_tick(PROPHILE_NSEC);

prophile_tick_t start = prophile_tick_rdtsc();
// ...
prophile_tick_t stop = prophile_tick_rdtsc();
```

## TODO

- [ ] Document and define the scope and intentions of the library.
- [ ] Should there be a PAUSE feature?
- [ ] Introduce some kind of mutex mechanism.
- [ ] Add additional LD_PRELOAD style injection support.
- [ ] Determine when \_\_declspec is needed.
- [ ] Use [STB](https://github.com/nothings/stb) if internal data management
  gets too complicated.
