cmake_minimum_required(VERSION 3.5)

enable_testing()
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(UtilityTest UtilityTest.cpp)
target_link_libraries(UtilityTest PUBLIC
    gtest_main
    coding_test_util
    ${JSONCPP_LIBRARIES} 
    ${CURL_LIBRARIES}
)

add_test(
    NAME UtilityTest
    COMMAND UtilityTest
)

