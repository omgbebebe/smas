cmake_minimum_required(VERSION 3.9)
project(smas VERSION 0.1.0 DESCRIPTION "SMart ASsistant GUI library")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF  CACHE BOOL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF  CACHE BOOL "Build the GLFW documentation")
set(GLFW_INSTALL OFF  CACHE BOOL "Generate installation target")
add_subdirectory("7thwok/glfw")

add_library(nanovg SHARED
	7thwok/nanovg/src/nanovg.c
	7thwok/nanovg/example/perf.c
)
target_include_directories(nanovg PRIVATE . 7thwok/nanovg/src/ 7thwok/nanovg/example/ )

add_library(smas SHARED
	smas.c
)

target_link_libraries(smas nanovg glfw GL)

set_target_properties(smas PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
	PUBLIC_HEADER smas.h
)

target_include_directories(smas PRIVATE . 7thwok/nanovg/src/ 7thwok/nanovg/example/ )

include(GNUInstallDirs)

install(TARGETS smas nanovg
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

configure_file(smas.pc.in smas.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/smas.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig
)

add_executable(smas_gui main.c)
target_include_directories(smas_gui PRIVATE .)
target_link_libraries(smas_gui smas)
