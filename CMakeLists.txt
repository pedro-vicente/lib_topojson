cmake_minimum_required(VERSION 2.8)
project (topojson)

if (MSVC)
  add_definitions( "/D_CRT_SECURE_NO_WARNINGS /D_WINSOCK_DEPRECATED_NO_WARNINGS /D_CRT_NONSTDC_NO_DEPRECATE" )
endif(MSVC)

include_directories(gason/src)
set(sources ${sources})
set(sources ${sources} gason/src/gason.h)
set(sources ${sources} gason/src/gason.cpp)
set(sources ${sources} parser.cc)
add_executable(parser ${sources})
