cmake_minimum_required(VERSION 2.8)
project(itktest)


find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

add_executable(itktest main.cpp ReadDicomSeriesTest.cpp)

target_link_libraries(itktest ${ITK_LIBRARIES})

install(TARGETS itktest RUNTIME DESTINATION bin)
