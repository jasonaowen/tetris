cmake_minimum_required (VERSION 3.5)
project (Tetris CXX)
add_executable (Tetris src/main.cpp
                       src/blocks.cpp
                       src/state.cpp)
add_executable (Test test/catch.cpp
                     src/blocks.cpp
                     src/state.cpp
                     test/state.cpp)
target_compile_features (Tetris PRIVATE cxx_generalized_initializers
                                        cxx_range_for
                                        cxx_strong_enums)
target_compile_features (Test PRIVATE cxx_generalized_initializers
                                      cxx_range_for
                                      cxx_strong_enums)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})

include (ExternalProject)
find_package (Git REQUIRED)
ExternalProject_ADD (
  catch
  PREFIX ${CMAKE_BINARY_DIR}/catch
  GIT_REPOSITORY https://github.com/philsquared/Catch.git
  TIMEOUT 10
  UPDATE_COMMAND ${GIT_EXECUTABLE} pull
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  LOG_DOWNLOAD ON
)
ExternalProject_Get_Property(catch source_dir)
set(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
enable_testing(true)
add_test(NAME catch
         COMMAND Test)
