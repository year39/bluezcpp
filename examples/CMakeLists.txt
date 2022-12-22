cmake_minimum_required(VERSION 3.8)

option(build_type_debug "Build Debug" ON)
option(generate_compile_commands "Generate compile_commands.json" ON)

if(${build_type_debug})
  set(CMAKE_BUILD_TYPE Debug)
  message("==> Build Type: Debug")
  message("==> Debug Symbols Generated")
else()
  set(CMAKE_BUILD_TYPE Release)
  message("==> Build Type: Release")
endif()

# generate compile_commands.json
if(${generate_compile_commands})
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
endif()


find_package(dbuscpp 1.0 CONFIG REQUIRED)
if(${dbuscpp_FOUND})
  message(STATUS "dbuscpp v${dbuscpp_VERSION} found")
  message(STATUS "dbuscpp directory: ${dbuscpp_DIR}")
else()
  message(FATAL_ERROR "dbuscpp not found")
endif()

find_package(bluezcpp 0.1 CONFIG REQUIRED)
if(${bluezcpp_FOUND})
  message(STATUS "bluezcpp v${bluezcpp_VERSION} found")
  message(STATUS "bluezcpp directory: ${bluezcpp_DIR}")
else()
  message(FATAL_ERROR "bluezcpp not found")
endif()

##################################################
# boost library
FIND_PACKAGE( Boost 1.73.0 REQUIRED )
IF (Boost_FOUND)
  message(STATUS "Boost v${Boost_VERSION} found")
  INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR})
endif()

add_executable(example src/example.cpp)
target_link_libraries(example bluezcpp::bluezcpp ${Boost_LIBRARIES})
target_compile_options(example PRIVATE -Wall -Wextra)
target_compile_features(example PRIVATE cxx_std_17)

add_executable(example_char src/example_char.cpp)
target_link_libraries(example_char bluezcpp::bluezcpp)
target_compile_options(example_char PRIVATE -Wall -Wextra)
target_compile_features(example_char PRIVATE cxx_std_17)
