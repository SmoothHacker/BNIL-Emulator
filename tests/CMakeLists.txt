add_executable(emu_tests_run tests.cpp
        registers.hpp)

include_directories(libemulator)

# Add unicorn engine for testing
find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "ENV{HOME}/Desktop/unicorn/INSTALL/lib/pkgconfig")
pkg_check_modules(Unicorn REQUIRED IMPORTED_TARGET GLOBAL unicorn)

target_compile_features(emu_tests_run PRIVATE cxx_std_23)
target_link_libraries(emu_tests_run PRIVATE libemulator PkgConfig::Unicorn GTest::gtest_main)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)
