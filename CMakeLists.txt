cmake_minimum_required(VERSION 3.28)


set(project_langs C CXX)

project(Tabiya
        LANGUAGES ${project_langs}
)

if(NOT DEFINED CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD
          20
          CACHE STRING "Default value for CXX_STANDARD property of targets."
  )
endif()

add_library(${PROJECT_NAME} INTERFACE
        src/Times.hpp
        src/concepts/concept.hpp
        src/IterWrapper.hpp
        src/Range.hpp)

target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_20)

add_subdirectory(tests)