cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(bnil_emulator)

# Create plugin executable target
include_directories(src)
add_library(libemulator src/emulator.cpp src/llil_visitor.cpp src/emulator.hpp src/llil_visitor.hpp src/plugin.cpp)
add_executable(bnil_emulator main.cpp)

target_compile_features(bnil_emulator PRIVATE cxx_std_23)
target_compile_features(libemulator PRIVATE cxx_std_23)
target_link_libraries(bnil_emulator libemulator)

set(CMAKE_CXX_FLAGS_RELEASE "-flto -march=native -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -fno-omit-frame-pointer -Og")

# Add vendored API dependency if enabled
option(BN_VENDOR_API "Use the Binary Ninja API submodule" ON)
if(BN_VENDOR_API)
	set(HEADLESS BOOL On)
	find_path(
		BN_API_PATH
		NAMES binaryninjaapi.h
		HINTS . .. ../.. binaryninja-api $ENV{BN_API_PATH}
		PATH_SUFFIXES binaryninja-api
		REQUIRED
	)
	message(STATUS "Found Binary Ninja API Path: ${BN_API_PATH}")
	add_subdirectory(${BN_API_PATH} api)
	target_link_libraries(libemulator binaryninjaapi)
	target_link_libraries(bnil_emulator binaryninjaapi)
endif()

add_subdirectory(tests)
