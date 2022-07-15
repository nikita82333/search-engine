cmake_minimum_required(VERSION 3.11)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

#static linking to make a standalone executable .exe file
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

include_directories("include/")
set(SRC_LIST src/ConverterJSON.cpp src/InvertedIndex.cpp src/SearchServer.cpp include/CustomExceptions.hpp)

include(FetchContent)
FetchContent_Declare(
        json
        URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz
)
FetchContent_MakeAvailable(json)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_disable_pthreads on)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(search_engine src/main.cpp ${SRC_LIST})
target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

enable_testing()

add_executable(tests tests/test1.cpp tests/test2.cpp ${SRC_LIST})
target_link_libraries(tests PRIVATE nlohmann_json::nlohmann_json gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
