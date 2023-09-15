cmake_minimum_required(VERSION 3.10)

set(LOCAL_PROJECT_NAME        "L1")
set(LOCAL_PROJECT_VERSION     "0.0.1")
set(LOCAL_PROJECT_DESCRIPTION "Description")

project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        DESCRIPTION ${LOCAL_PROJECT_DESCRIPTION}
        LANGUAGES CXX)


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${LOCAL_PROJECT_NAME})

target_link_directories(${LOCAL_PROJECT_NAME} PRIVATE ${GTKMM_LIBRARY_DIRS})
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${GTKMM_INCLUDE_DIRS} include)
target_sources(${LOCAL_PROJECT_NAME} PRIVATE ${SOURCES})
target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE ${GTKMM_CFLAGS_OTHER})
