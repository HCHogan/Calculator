# Case insensitive, so the common practice is to use the lower case.
cmake_minimum_required(VERSION 3.7...3.27)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
  cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(Calculator VERSION 1.0
                 DESCRIPTION "Very nice project"
                 LANGUAGES CXX)

include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v5.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
  Eigen
  GIT_REPOSITORY https://gitlab.com/libeigen/eigen.git
  GIT_TAG 3.4.0
)
FetchContent_MakeAvailable(Eigen)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(calclib
  STATIC
  src/calculate.cpp
)
target_include_directories(calclib
  PUBLIC
  include
)

add_library(calc_intlib
  STATIC
  src/calculate_int.cpp
)
target_include_directories(calc_intlib
  PUBLIC
  include
)

add_executable(calculator
  apps/main.cpp
)
target_include_directories(calculator PUBLIC ${ftxui_SOURCE_DIR}/include)
target_link_libraries(calculator PUBLIC calclib)
target_link_libraries(calculator PUBLIC Eigen3::Eigen)
target_link_libraries(calculator PUBLIC ftxui::dom ftxui::screen ftxui::component)
