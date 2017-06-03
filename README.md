# BigWord

An application to efficiently find words in a dictionary from a multiset of letters.

[![Build Status](https://travis-ci.org/mafagafogigante/bigword.svg?branch=travis)](https://travis-ci.org/mafagafogigante/bigword)

## Running

```
$ ./bigword --help
> Usage: ./bigword [OPTIONS] LETTERS
>     --configuration  Output the option values.
>     --copyright      Output the license.
>     --line-number    Output line numbers.
>     --source         Set the source file.
>     --time           Output computation times.
>     --version        Output the program version.

$ ./bigword linkedlists
> instilled
> kindliest
> niellists
> slinkiest

$ ./bigword abcdefghij
> bighead
> jibhead

$ ./bigword aeioubsdlinux
> unoxidisable

$ ./bigword --time anythingyousynthesize irks lock padlock
> -- Finished loading after 60 milliseconds.
> -- Query 1 matched 1 word after 2 milliseconds.
> synthesization
> -- Query 2 matched 4 words after 248 microseconds.
> irks
> kirs
> kris
> risk
> -- Query 3 matched 2 words after 294 microseconds.
> colk
> lock
> -- Query 4 matched 1 word after 1 millisecond.
> padlock
```

## Building

```
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ..
$ make
```

### Compilers

This project has been tested with Clang, GCC, and the Intel C++ Compiler.

No compiler greatly outperformed the alternatives.

## Design

Data is serialized after processing in order to speed up subsequent runs.

### Serialization

Human-readable formats are preferred.

Most of the data is plain text, storing it as binary does not improve
performance significantly.

When a string may have spaces, it is dumped after a 32-bit unsigned integer
value which represents the string size. When it is safe to assume that the
string does not have spaces, it is dumped in a new line all by itself.
