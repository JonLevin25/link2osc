cmake_minimum_required(VERSION 3.0)

set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment version")
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build universal binary")

project(Carabiner)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(Carabiner_VERSION "1.1.6")
configure_file(carabiner.h.in carabiner.h)
configure_file(Info.plist.in Info.plist)

set(LIBLO_TARGET lo_shared)
# set(LIBLO_TARGET lo_static)

include(link/AbletonLinkConfig.cmake)
add_subdirectory(gflags)
add_subdirectory(liblo/cmake)


set(mongoose_HEADERS
  mongoose.h
)

set(mongoose_SOURCES
  mongoose.c
)

add_executable(Carabiner
  ${mongoose_HEADERS}
  ${mongoose_SOURCES}
  carabiner.cpp
)

target_include_directories(Carabiner PUBLIC "${PROJECT_BINARY_DIR}")

if (APPLE)
   target_link_options(Carabiner PRIVATE LINKER:-sectcreate,__TEXT,__info_plist,${PROJECT_BINARY_DIR}/Info.plist)
endif()

target_link_libraries(Carabiner Ableton::Link gflags)
target_link_libraries(Carabiner ${LIBLO_TARGET})

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  target_link_libraries(Carabiner pthread)
endif()
