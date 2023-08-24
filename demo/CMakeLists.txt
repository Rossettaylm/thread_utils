cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(demo)

aux_source_directory(${PROJECT_SOURCE_DIR}/src demo_source)

add_executable(demo ${demo_source})

link_directories(${CMAKE_OUTPUT_DIRECTORY}/bin)

target_link_libraries(demo thread_lib)

target_include_directories(
  demo
  PRIVATE
  ./include
  ${CMAKE_SOURCE_DIR}/thread/include
)
