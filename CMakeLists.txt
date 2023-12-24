cmake_minimum_required(VERSION 3.10)

project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        LANGUAGES CXX)


set(LOCAL_PROJECT_NAME "L1")
set(LOCAL_PROJECT_VERSION "0.0.1")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_STATIC_LIBRARIES TRUE)

set(CIMG_NEED_X11 1)
set(CIMG_NEED_PNG 1)
set(CIMG_NEED_JPG 1)

set(CIMG_H_PATH "${PROJECT_SOURCE_DIR}/Libraries/CImg")


file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/include/*.hpp")
    SET(dir_list "")
    FOREACH (file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH ()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

add_executable(${LOCAL_PROJECT_NAME} ${SOURCES})
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${header_dir_list})


# ---SFML---
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${SFML_INCLUDE_DIRS})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics)


# ---OpenMP---
find_package(OpenMP)
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)


# ---CImg---
add_library(CImg_Lib INTERFACE)
target_include_directories(CImg_Lib INTERFACE ${CIMG_H_PATH})
target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE CImg_Lib)

if(CIMG_NEED_X11)
    message(STATUS "Looking for X11...")
    find_package(X11 REQUIRED)
    target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${X11_INCLUDE_DIR})
    target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE ${X11_LIBRARIES})
else()
    target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE cimg_display=0)
endif()

if(CIMG_NEED_JPG)
    message(STATUS "Looking for libjpg...")
    find_package(JPEG REQUIRED)
    target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${JPEG_INCLUDE_DIR})
    target_link_libraries (${LOCAL_PROJECT_NAME} PRIVATE ${JPEG_LIBRARY})
    target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE cimg_use_jpeg=1)
endif()

if(CIMG_NEED_PNG)
    message(STATUS "Looking for libpng...")
    find_package(PNG REQUIRED)
    target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${PNG_INCLUDE_DIR})
    target_link_libraries (${LOCAL_PROJECT_NAME} PRIVATE ${PNG_LIBRARY})
    target_compile_definitions(${LOCAL_PROJECT_NAME} PRIVATE cimg_use_png=1)
endif()
