find_package(Catch2 3 REQUIRED)
add_executable(emu_tests_run Add.cpp)

include_directories(libemulator)
target_compile_features(emu_tests_run PRIVATE cxx_std_23)
target_link_libraries(emu_tests_run PRIVATE libemulator)
target_link_libraries(emu_tests_run PRIVATE Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(emu_tests_run)