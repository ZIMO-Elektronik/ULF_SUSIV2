include(GoogleTest)

file(GLOB_RECURSE SRC *.cpp)
add_executable(ULF_SUSIV2Tests ${SRC})

sanitize(address,undefined)

target_common_warnings(ULF_SUSIV2Tests PRIVATE)
target_common_errors(ULF_SUSIV2Tests PRIVATE -Werror)

cpmaddpackage(URI "gh:google/googletest#main" OPTIONS "INSTALL_GTEST OFF")

target_link_libraries(ULF_SUSIV2Tests PRIVATE ULF_SUSIV2 GTest::gtest_main
                                              GTest::gmock)

gtest_discover_tests(ULF_SUSIV2Tests)
