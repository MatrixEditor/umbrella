cmake_minimum_required(VERSION 3.10)
project(umbrella CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /EHsc /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /EHsc /MTd")
endif()

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

find_package(Python 3.11 COMPONENTS Interpreter Development.Module REQUIRED)

set(PACKAGE_NAME umbrella-cxx)

execute_process(
  COMMAND "${Python_EXECUTABLE}" -m nanobind --cmake_dir
  OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE NB_DIR)

message(STATUS "Nanobind DIR: ${NB_DIR} using ${Python_EXECUTABLE} -m nanobind --cmake_dir")
list(APPEND CMAKE_PREFIX_PATH "${NB_DIR}")

find_package(nanobind CONFIG REQUIRED)

file(GLOB_RECURSE PY_ALL_SOURCE_FILES "src/*.cpp")

nanobind_add_module(${PACKAGE_NAME}
  NB_STATIC
  ${PY_ALL_SOURCE_FILES}
)

target_include_directories(${PACKAGE_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)
target_include_directories(${PACKAGE_NAME}
  SYSTEM PRIVATE
  ${PYBIND11_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS}
)

add_subdirectory(../../ umbrella)
target_link_libraries(${PACKAGE_NAME} PUBLIC umbrella::umbrella umbrella::LIEF)

set_target_properties(${PACKAGE_NAME} PROPERTIES OUTPUT_NAME _umbrella)

if (WIN32)
  target_link_libraries(${PACKAGE_NAME} PUBLIC ${PYTHON_LIBRARIES})
endif()

if (MSVC)
  add_custom_command(TARGET ${PACKAGE_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:${PACKAGE_NAME}> ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()