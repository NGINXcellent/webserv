cmake_minimum_required(VERSION 3.14)
project(my_project)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b73f27fd164456fea9aba56163f5511355a03272.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(This webserv)

project(${This} CXX)

# GoogleTest requires C++
set(CMAKE_CXX_STANDARD 14)

enable_testing()

set(Includes
    ../../include/config/InputHandler.hpp
    ../../include/http/HttpResponse.hpp
    ../../include/http/HttpRequestFactory.hpp
    ../../include/http/HttpRequest.hpp
    ../../include/http/HttpTime.hpp
    ../../include/http/HttpStatus.hpp
    ../../include/http/MimeType.hpp
    ../../include/http/Server.hpp
    ../../include/io/Client.hpp
)

set(Sources
	../../src/main.cpp
    ../../src/config/InputHandler.cpp
    ../../src/http/HttpResponse.cpp
    ../../src/http/HttpRequest.cpp
    ../../src/http/HttpTime.cpp
    ../../src/http/HttpStatus.cpp
    ../../src/http/HttpRequestFactory.cpp
    ../../src/http/MimeType.cpp
    ../../src/http/Server.cpp
    ../../src/io/Client.cpp
)

# add_library(${This} ${Sources} ${Libft} ${Headers})
add_library(${This} ${Sources} ${Includes})

# this will add all the subdirectories with unit tests
add_subdirectory(InputTests)
add_subdirectory(RequestTests)
add_subdirectory(ResponseTests)
add_subdirectory(FileTests)
add_subdirectory(HttpTests)
add_subdirectory(IOTests)
