cmake_minimum_required(VERSION 3.5)

add_compile_options(-std=c++11)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)


find_package(Qt5 COMPONENTS Widgets PrintSupport REQUIRED)
find_package(QCustomPlot)


add_executable(sensor_simulation
  main.cpp
  simplesimulation.cpp
  simplesimulation.h
  simplesimulation.ui
)


target_link_libraries(sensor_simulation 
  Qt5::Widgets 
  Qt5::PrintSupport 
  ${QCustomPlot_LIBRARIES}
  ${JSONCPP_LIBRARIES}
  ${CURL_LIBRARIES}
  coding_test_util
)

install(TARGETS sensor_simulation)
