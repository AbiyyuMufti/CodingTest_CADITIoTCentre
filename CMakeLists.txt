cmake_minimum_required(VERSION 3.5)
project(coding_test C CXX)

add_compile_options(-std=c++11)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(PkgConfig REQUIRED)
find_package(CURL REQUIRED)

pkg_check_modules(JSONCPP jsoncpp)
link_libraries(${JSONCPP_LIBRARIES})

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${JSONCPP_LIBRARIES}
    ${CURL_INCLUDE_DIR}
)

add_library(coding_test_util STATIC src/utility.cpp include/utility.hpp)
target_link_libraries(coding_test_util ${JSONCPP_LIBRARIES} ${CURL_LIBRARIES})

add_executable(salary_conversion src/salary_conversion.cpp)
target_link_libraries(salary_conversion coding_test_util ${JSONCPP_LIBRARIES} ${CURL_LIBRARIES})

add_executable(sensor_aggregation src/sensor_aggregation.cpp)
target_link_libraries(sensor_aggregation coding_test_util ${JSONCPP_LIBRARIES} ${CURL_LIBRARIES})

add_subdirectory(sensor_simulation)
add_subdirectory(test)

install(TARGETS salary_conversion)
install(TARGETS sensor_aggregation)


