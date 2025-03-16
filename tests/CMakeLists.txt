find_package(Catch2 3 REQUIRED)
add_executable(emu_tests_run tests.cpp
        registers.hpp)

include_directories(libemulator)

# Add unicorn engine for testing
find_package(PkgConfig REQUIRED)
pkg_check_modules(Unicorn REQUIRED IMPORTED_TARGET GLOBAL unicorn)


target_compile_features(emu_tests_run PRIVATE cxx_std_23)
target_link_libraries(emu_tests_run PRIVATE libemulator PkgConfig::Unicorn)
target_link_libraries(emu_tests_run PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(emu_tests_run)