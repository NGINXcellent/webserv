cmake_minimum_required(VERSION 3.14)

set(this FileTests)

set(Sources 
    MimeTypeTests.cpp)

add_executable(${this} ${Sources})

target_link_libraries(${this} PUBLIC
	GTest::gtest_main
    webserv
)

add_test(
		NAME ${this}
		COMMAND ${this}
	)

