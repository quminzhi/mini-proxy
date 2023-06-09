cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_BUILD_TYPE Debug)

set(LIB_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

###########################
# add your own library here
###########################
add_library(
  comm SHARED
  include/comm.h
  src/comm.c
)
target_include_directories(comm PUBLIC "${LIB_INCLUDE_DIR}")

add_library(
  nio SHARED
  include/nio.h
  src/nio.c
)
target_include_directories(nio PUBLIC "${LIB_INCLUDE_DIR}")
target_link_libraries(nio PUBLIC 
  csapp
)

add_library(
  cache SHARED
  include/cache.h
  src/cache.c
)
target_include_directories(cache PUBLIC "${LIB_INCLUDE_DIR}")
target_link_libraries(cache PUBLIC 
  comm
  csapp
)

add_library(
  inet-helper SHARED
  include/inet-helper.h
  src/inet-helper.c
)
target_include_directories(inet-helper PUBLIC "${LIB_INCLUDE_DIR}")

add_library(
  proxy-module SHARED
  include/proxy-module.h
  src/proxy-module.c
)
target_include_directories(proxy-module PUBLIC "${LIB_INCLUDE_DIR}")
target_link_libraries(proxy-module PUBLIC 
  csapp
  comm
  inet-helper
  cache
  nio
)

add_library(
  sbuf SHARED
  include/sbuf.h
  src/sbuf.c
)
target_include_directories(sbuf PUBLIC "${LIB_INCLUDE_DIR}")
target_link_libraries(sbuf PUBLIC 
  csapp
)

###########################
# external link_libraries
###########################

# add a shared library as the external library, csapp
add_library(
  csapp SHARED
  include/csapp.h
  src/csapp.c
)
target_include_directories(csapp PUBLIC "${LIB_INCLUDE_DIR}")
target_link_libraries(csapp -lpthread)

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
