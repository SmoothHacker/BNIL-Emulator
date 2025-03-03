cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

project(bnil_emulator)

# Create plugin executable target
file(GLOB BN_API_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)
add_executable(bnil_emulator ${BN_API_SOURCES})

target_compile_features(bnil_emulator PRIVATE cxx_std_17)

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
	target_link_libraries(bnil_emulator binaryninjaapi)
endif()

