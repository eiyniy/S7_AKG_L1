cmake_minimum_required(VERSION 3.10)

set(LOCAL_PROJECT_NAME        "L1")
set(LOCAL_PROJECT_VERSION     "0.0.1")

project(${LOCAL_PROJECT_NAME}
        VERSION ${LOCAL_PROJECT_VERSION}
        LANGUAGES CXX)

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list "${PROJECT_SOURCE_DIR}/include/*.hpp")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES(header_dir_list)

list(LENGTH header_dir_list header_dir_list_count)
message(STATUS "[INFO] Found ${header_dir_list_count} header directories.")

# include(FetchContent)
# set(BUILD_SHARED_LIBS OFF)
# FetchContent_Declare(
#  SFML
#  GIT_REPOSITORY https://github.com/SFML/SFML.git
#  GIT_TAG 2.6.0
# )
# FetchContent_MakeAvailable(SFML)

set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML 2.6 COMPONENTS system window graphics REQUIRED)

include(FetchContent)
FetchContent_Declare(
    PVS_CMakeModule
    GIT_REPOSITORY "https://github.com/viva64/pvs-studio-cmake-module.git"
    GIT_TAG        "master" 
)
FetchContent_MakeAvailable(PVS_CMakeModule)
include("${pvs_cmakemodule_SOURCE_DIR}/PVS-Studio.cmake")

include_directories(${SFML_INCLUDE_DIRS})

add_executable(${LOCAL_PROJECT_NAME} ${SOURCES})

target_link_libraries(${LOCAL_PROJECT_NAME} PRIVATE sfml-system sfml-window sfml-graphics)
target_include_directories(${LOCAL_PROJECT_NAME} PRIVATE ${header_dir_list})

# include(../pvs-studio-cmake-module/PVS-Studio.cmake)
pvs_studio_add_target(TARGET example2.analyze ALL
                      OUTPUT FORMAT errorfile
                      ANALYZE ${PROJECT_NAME}
                      LOG target.err)