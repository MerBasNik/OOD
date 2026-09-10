add_executable(SimUDuck
        lib/SimUDuck.cpp
        ducks/Ducks.cpp
)

target_include_directories(SimUDuck PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Fly
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Quack
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Dance
        ${CMAKE_CURRENT_SOURCE_DIR}/ducks
)

add_executable(SimUDuck_tests
        tests/DanceTest.cpp
        ducks/Ducks.cpp
)

target_include_directories(SimUDuck_tests PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Fly
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Quack
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Dance
        ${CMAKE_CURRENT_SOURCE_DIR}/ducks
)

target_link_libraries(SimUDuck_tests PRIVATE
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(SimUDuck_tests)

if (TARGET format-code)
    add_dependencies(SimUDuck_tests format-code)
endif ()