# Test Driven Framework

This is a test-driven framework built on Google Test.

## Structure

CMake is a tree-like auto compilation system. Test driven framework is
constructed and organized with CMake and Google Test. It supports software
development in C and C++.

- Google Test

[Google Test](https://github.com/google/googletest) is a test framework
developed by Google Inc. in C++. Google Test is compiled as a dynamic linked
(shared) library and ingrated in our test driven framework for software
development.

```cmake
set(EXTERNALTEST external-test)
set(SOURCES external-test.cpp)
add_executable(${EXTERNALTEST} ${SOURCES})
target_link_libraries(${EXTERNALTEST} PUBLIC 
  gtest_main   # <===
  libexternal 
)
add_test(NAME ${EXTERNALTEST} COMMAND "${EXTERNALTEST}")``cmake
```

- User-defined Library

User-defined library is defined in `lib` folder, which includes `src` and
`include` folders. `src` folder contains all sorts of source files (e.g.
`file.c`, `file.cpp`, and so forth).

In addition to editing source and header files, we need to tell compilation
system how to incorporate our libraries into the whole project by
`CMakefile.txt`.

```cmake
# add your own library here
add_library(
  foo SHARED
  include/foo.h
  src/foo.c
)
target_include_directories(foo PUBLIC "${LIB_INCLUDE_DIR}")
```

- Third-party library

Third party library and user-defined library share `lib` folder. The only
difference is that we have to aggregate them into external dynamic linked
library to provide a unified interface for client programs. The demo code below 
shows how to compile `csapp` and expose it with a unified interface `libexternal`.

```cmake
# external link_libraries
add_library(
  csapp SHARED
  include/csapp.h
  src/csapp.c
)
target_include_directories(csapp PUBLIC "${LIB_INCLUDE_DIR}")

# aggregate all auxiliary libraries into libcomm 
# todo: add library to import in libcomm
add_library(
  libexternal INTERFACE
)

target_link_libraries(
  libexternal INTERFACE
  csapp
)
target_include_directories(libexternal INTERFACE "${LIB_INCLUDE_DIR}")
```

- Test

The functionality of libraries (`.so` in linux or `.dll` in windows) is tested
with test files in `test` folder. Test is processed with Google Test framework.

```c
#include <gtest/gtest.h>

extern "C" {
#include <stdlib.h>
}

class FooTest: public ::testing::Test {
protected:

  void SetUp() override {
  }

  void TearDown() override {}
};

TEST_F(FooTest, TestWordReference) {
  // do your test
}
```

- App

The final application is produced in `app` folder as an executable file, which
can be published and used for whatever uses.

## Features

- A test-driven software development framework
- A rich set of test
- A c++ framework and c compatible.
- Test and development separation.

## Contributing Changes

Contributions are welcomed and please send your changes as the form as pull
request (PR).

Contact: quminzhi@gmail.com
