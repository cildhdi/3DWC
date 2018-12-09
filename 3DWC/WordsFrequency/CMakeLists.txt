cmake_minimum_required(VERSION 3.0.0)
project(WordsFrequency)


include_directories(include)
include_directories(.)

# config cpr
set(USE_SYSTEM_CURL ON)
set(BUILD_CPR_TESTS OFF)
add_subdirectory(cpr)
include_directories(${CPR_INCLUDE_DIRS})

find_package(rapidjson REQUIRED)
include_directories(${RAPIDJSON_INCLUDE_DIRS})
find_package(glog REQUIRED)

aux_source_directory(src WF_SRC)
add_library(WordsFrequency ${WF_SRC})
target_link_libraries(WordsFrequency ${CPR_LIBRARIES} glog::glog)

add_executable(wftest test.cpp)
target_link_libraries(wftest WordsFrequency)
