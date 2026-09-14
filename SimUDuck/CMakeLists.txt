add_executable(SimUDuck
        SimUDuck.cpp
)

target_include_directories(SimUDuck PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/behavior
        ${CMAKE_CURRENT_SOURCE_DIR}/duck
)

add_executable(SimUDuck_tests
        tests/DuckTest.cpp
        tests/FlyActionTest.cpp
)

target_include_directories(SimUDuck_tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/behavior
        ${CMAKE_CURRENT_SOURCE_DIR}/duck
)

target_link_libraries(SimUDuck_tests PRIVATE
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
        GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(SimUDuck_tests)

if (TARGET format-code)
    add_dependencies(SimUDuck_tests format-code)
endif ()