cmake_minimum_required(VERSION 3.28)
project(test LANGUAGES CXX)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  option(CXX_STANDARD_REQUIRED "Require c++ standard" YES)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_EXTENSIONS NO)
endif()

enable_testing()

find_package(Catch2 3 REQUIRED)

add_executable(TimesUTests TimesUTests.cpp)
target_link_libraries(TimesUTests PRIVATE Catch2::Catch2WithMain Tabiya)

add_executable(IterWrapUTests IterWrapUTests.cpp)
target_link_libraries(IterWrapUTests PRIVATE Catch2::Catch2WithMain Tabiya)

add_executable(RangeTests RangeUTests.cpp)
target_link_libraries(RangeTests PRIVATE Catch2::Catch2WithMain Tabiya)