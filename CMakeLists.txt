cmake_minimum_required(VERSION 3.20)
project(EditorApp C)

set(EDITOR_APP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(EDITOR_APP_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(EDITOR_APP_VERSION_MAJOR 1)
set(EDITOR_APP_VERSION_MINOR 0)
set(EDITOR_APP_VERSION_PATCH 0)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MY_PKG REQUIRED IMPORTED_TARGET gtk4)

set(GLIB_FLAGS --target=resources.c)
set(GLIB_FLAGS ${GLIB_FLAGS} --generate-source)
set(GLIB_FLAGS ${GLIB_FLAGS} --sourcedir=${PROJECT_SOURCE_DIR})

add_custom_command(
        OUTPUT resources.c
        COMMAND glib-compile-resources ${PROJECT_SOURCE_DIR}/resource.xml ${GLIB_FLAGS}
        DEPENDS ${PROJECT_SOURCE_DIR}/resource.xml ${PROJECT_SOURCE_DIR}/window.ui
        ${PROJECT_SOURCE_DIR}/window.ui
)

add_custom_target(generate_resources DEPENDS resources.c)

set(SOURCE_FILES main.c ${CMAKE_CURRENT_BINARY_DIR}/resources.c)


add_executable(${PROJECT_NAME} ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} generate_resources)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

target_link_libraries(${PROJECT_NAME} ${GTK_LIBRARIES})