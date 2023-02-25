cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(thread_lib)

aux_source_directory(${PROJECT_SOURCE_DIR}/src thread_lib_source)

add_library(
  thread_lib
  SHARED
  ${thread_lib_source}
)

