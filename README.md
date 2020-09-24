# Generators

Toy generator framework with the generators implemented as C++17 lambdas and
a facade template to provide a higher level API to them.

## Generating the build

You've used a cookiecutter to generate this project, but there's
another step to get started - this project is based on CMake, so you
need to use this CMake build to generate a build that will work on
your system.

This build structure can be done "out of tree" (ie not in this folder
structure), or use the .build/ folder that's automatically created -
this is also in the .gitignore by default, if you want

A typical CMake invocation (for Clang and C++17) would be:

```shell
$ cd build
$ cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="-std=c++17" \
  -DCMAKE_CXX_FLAGS_DEBUG="-fstandalone-debug -ggdb -O0" ..
```

Then run the makefile as (for example):

```shell
$ make
```

## License(s)

The license for this can be found in the LICENSE.md file, - but
be advised that the Google Test library found in the lib folder has
its own license terms. Please read that license from Google relating
specifically to Google Test and Google Mock.
