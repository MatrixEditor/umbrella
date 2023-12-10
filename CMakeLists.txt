cmake_minimum_required(VERSION 3.15)

include(CMakePackageConfigHelpers)

# Add CMake modules to path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/.git"
   AND IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/.git")
  find_package(Git REQUIRED)
  include(gitConfig)
else()
  set(UMBRELLA_VERSION_MAJOR "1")
  set(UMBRELLA_VERSION_MINOR "0")
  set(UMBRELLA_VERSION_PATCH "0")
endif()


project(umbrella
  VERSION ${UMBRELLA_VERSION_MAJOR}.${UMBRELLA_VERSION_MINOR}.${UMBRELLA_VERSION_PATCH}
  LANGUAGES CXX
)

configure_file(
  "${PROJECT_SOURCE_DIR}/include/umbrella/version.h.in"
  "${PROJECT_SOURCE_DIR}/include/umbrella/version.h"
)

configure_file(
  "${PROJECT_SOURCE_DIR}/bindings/python/setup.py.in"
  "${PROJECT_SOURCE_DIR}/bindings/python/setup.py"
)

configure_file(
  "${PROJECT_SOURCE_DIR}/Doxyfile.in"
  "${PROJECT_SOURCE_DIR}/Doxyfile"
)

add_library(umbrella)
add_library(umbrella::umbrella ALIAS umbrella)

# Basic source files
target_sources(umbrella
  PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}/src/runtime.cpp
  ${CMAKE_CURRENT_LIST_DIR}/src/MachOStream.cpp
)

# Objective-C ABI
target_sources(umbrella
  PRIVATE
  src/objc/Class.cpp
  src/objc/IVar.cpp
  src/objc/ABI.cpp
  src/objc/Method.cpp
  src/objc/Category.cpp
  src/objc/Property.cpp
  src/objc/Protocol.cpp
  src/objc/TypeEncoding.cpp
)

target_include_directories(umbrella
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

find_package(LIEF 0.13.2 REQUIRED COMPONENTS STATIC)
add_library(LIEF INTERFACE)
add_library(umbrella::LIEF ALIAS LIEF)

target_link_libraries(LIEF INTERFACE $<INSTALL_INTERFACE:$<INSTALL_PREFIX>/lib/libLIEF${CMAKE_STATIC_LIBRARY_SUFFIX}>)
target_link_libraries(umbrella PRIVATE $<BUILD_INTERFACE:LIEF::LIEF>)

get_target_property(_LIEF_INC  LIEF::LIEF INTERFACE_INCLUDE_DIRECTORIES)
get_target_property(_LIEF_CONF LIEF::LIEF IMPORTED_CONFIGURATIONS)
get_target_property(_LIEF_LIB  LIEF::LIEF IMPORTED_LOCATION_${_LIEF_CONF})

target_include_directories(umbrella SYSTEM PUBLIC ${_LIEF_INC})
set_target_properties(umbrella PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

message(STATUS "LIEF Lib      : ${_LIEF_LIB}")
message(STATUS "LIEF Includes : ${_LIEF_INC}")