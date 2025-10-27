cmake_minimum_required(VERSION 3.25 FATAL_ERROR)
include(FetchContent)

FetchContent_Declare(
  CMakeModules
  GIT_REPOSITORY "https://github.com/ZIMO-Elektronik/CMakeModules"
  GIT_TAG v0.10.0
  SOURCE_DIR ${CMAKE_BINARY_DIR}/CMakeModules)
FetchContent_MakeAvailable(CMakeModules)

version_from_git()
project(
  ULF_SUSIV2
  VERSION ${VERSION_FROM_GIT}
  LANGUAGES CXX)

set(ULF_SUSIV2_MAX_FRAME_SIZE
    268u
    CACHE STRING "Maximum size of a frame in bytes")
set(ULF_SUSIV2_MAX_RESPONSE_SIZE
    6u
    CACHE STRING "Maximum size of a response in bytes")

add_library(ULF_SUSIV2 INTERFACE ${SRC})
add_library(ULF::SUSIV2 ALIAS ULF_SUSIV2)

target_compile_features(ULF_SUSIV2 INTERFACE cxx_std_23)

target_compile_definitions(
  ULF_SUSIV2
  INTERFACE ULF_SUSIV2_MAX_FRAME_SIZE=${ULF_SUSIV2_MAX_FRAME_SIZE}
            ULF_SUSIV2_MAX_RESPONSE_SIZE=${ULF_SUSIV2_MAX_RESPONSE_SIZE})

if(PROJECT_IS_TOP_LEVEL)
  target_include_directories(ULF_SUSIV2 INTERFACE include)
  target_common_warnings(ULF_SUSIV2 INTERFACE)
  target_common_errors(ULF_SUSIV2 INTERFACE)
else()
  target_include_directories(ULF_SUSIV2 SYSTEM INTERFACE include)
endif()

if(NOT TARGET ZTL::ZTL)
  cpmaddpackage("gh:ZIMO-Elektronik/ZTL@0.23.1")
endif()

if(NOT TARGET ZUSI::ZUSI)
  cpmaddpackage("gh:ZIMO-Elektronik/ZUSI@0.9.3")
endif()

target_link_libraries(ULF_SUSIV2 INTERFACE ZTL::ZTL ZUSI::ZUSI)

if(PROJECT_IS_TOP_LEVEL)
  include(CTest)
  # add_subdirectory(examples)
  file(
    DOWNLOAD
    "https://github.com/ZIMO-Elektronik/.github/raw/master/data/.clang-format"
    ${CMAKE_CURRENT_LIST_DIR}/.clang-format)
  file(GLOB_RECURSE SRC include/*.*pp tests/*.*pp)
  add_clang_format_target(ULF_SUSIV2Format OPTIONS -i FILES ${SRC})
endif()

if(BUILD_TESTING
   AND PROJECT_IS_TOP_LEVEL
   AND CMAKE_SYSTEM_NAME STREQUAL CMAKE_HOST_SYSTEM_NAME)
  add_subdirectory(tests)
endif()
