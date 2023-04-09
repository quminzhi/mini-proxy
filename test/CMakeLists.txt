cmake_minimum_required(VERSION 3.18.4)

set(CMAKE_BUILD_TYPE Debug)

#################################
# Add a test for shared libraries
#################################

# add a test for proxy-module
set(PROXYTEST proxy-module-test)
set(SOURCES proxy-module-test.cpp)
add_executable(${PROXYTEST} ${SOURCES})
target_link_libraries(${PROXYTEST} PUBLIC 
  gtest_main 
  proxy-module
)
add_test(NAME ${PROXYTEST} COMMAND "${PROXYTEST}")

# # add a test for external link_libraries
# set(EXTERNALTEST external-test)
# set(SOURCES external-test.cpp)
# add_executable(${EXTERNALTEST} ${SOURCES})
# target_link_libraries(${EXTERNALTEST} PUBLIC 
#   gtest_main 
#   libexternal 
# )
# add_test(NAME ${EXTERNALTEST} COMMAND "${EXTERNALTEST}")
