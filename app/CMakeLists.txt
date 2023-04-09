cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_BUILD_TYPE Debug)

#############################
# add other apps here
#############################
set(APP proxy)
add_executable(${APP} ${APP}.c)
target_link_libraries(${APP} PUBLIC
  # link libraries needed
  libexternal
  inet-helper
  proxy-module
) 

add_executable(client client.c)
target_link_libraries(client PUBLIC
  # link libraries needed
  libexternal
  comm
  inet-helper
) 
