cmake_minimum_required(VERSION 2.6)
project(itktest)

add_executable(itktest main.cpp)

install(TARGETS itktest RUNTIME DESTINATION bin)
