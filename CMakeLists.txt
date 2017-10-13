cmake_minimum_required(VERSION 2.6)

project(joytime-core)

include(GenerateExportHeader)

add_library(joytime-core SHARED "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-core.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/controller.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/rumble.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-core-wrapper.cpp")
add_library(joytime-core_static STATIC "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-core.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/controller.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/rumble.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/joytime-core-wrapper.cpp")

set_target_properties(joytime-core PROPERTIES
  #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  CXX_VISIBILITY_PRESET hidden
  #BUILD_TYPE Release
)
set_target_properties(joytime-core_static PROPERTIES
  #ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  #LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  CXX_VISIBILITY_PRESET hidden
  #BUILD_TYPE Release
)

GENERATE_EXPORT_HEADER(joytime-core
  BASE_NAME joytime_core
  EXPORT_MACRO_NAME JOYTIME_CORE_EXPORT
  EXPORT_FILE_NAME "${CMAKE_CURRENT_SOURCE_DIR}/include/joytime_core_EXPORTS.h"
  STATIC_DEFINE JOYTIME_CORE_BUILT_AS_STATIC
)

# EventEmitter include
target_include_directories(joytime-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/cpp-EventEmitter")
target_include_directories(joytime-core_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/deps/cpp-EventEmitter")

target_include_directories(joytime-core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(joytime-core_static PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_compile_definitions(joytime-core_static PUBLIC JOYTIME_CORE_BUILT_AS_STATIC=1)

if (NOT MSVC)
  target_compile_options(joytime-core PRIVATE "-Wno-c++11-narrowing")
  target_compile_options(joytime-core_static PRIVATE "-Wno-c++11-narrowing")
endif (MSVC)