add_executable(Paint
        main.cpp
        Shape/Shape.cpp
        Picture/Picture.cpp
        Canvas/Canvas.cpp
        Lib/Parser/Parser.cpp
        Behavior/CircleBehavior.cpp
        Behavior/LineBehavior.cpp
        Behavior/RectangleBehavior.cpp
        Behavior/TriangleBehavior.cpp
        Behavior/TextBehavior.cpp
        Command/AddCommand/AddCommand.cpp
        Command/ChangeColor/ChangeColor.cpp
        Command/DrawCommand/DrawCommand.cpp
        Command/CloneCommand/CloneCommand.cpp
        Command/ChangeCommand/ChangeCommand.cpp
        Command/DeleteCommand/DeleteCommand.cpp
        Command/MoveCommand/MoveCommand.cpp
        Command/MovePictureCommand/MovePictureCommand.cpp
        Command/ListCommand/ListCommand.cpp
        Command/DrawPictureCommand/DrawPictureCommand.cpp
        Lib/Parser/ParseBehavior.cpp
)

target_include_directories(Paint PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Behavior
        ${CMAKE_CURRENT_SOURCE_DIR}/Canvas
        ${CMAKE_CURRENT_SOURCE_DIR}/Lib
        ${CMAKE_CURRENT_SOURCE_DIR}/Shape
        ${CMAKE_CURRENT_SOURCE_DIR}/Picture
        ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Parser
        ${CMAKE_CURRENT_SOURCE_DIR}/Command
)

#add_executable(SimUDuck_tests
#        tests/DanceTest.cpp
#        tests/FliesCountTest.cpp
#        ducks/Ducks.cpp
#)

#target_include_directories(SimUDuck_tests PUBLIC
#        ${CMAKE_CURRENT_SOURCE_DIR}
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Fly
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Quack
#        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Duck/Dance
#        ${CMAKE_CURRENT_SOURCE_DIR}/ducks
#)

#target_link_libraries(SimUDuck_tests PRIVATE
#        GTest::gtest
#        GTest::gmock
#        GTest::gtest_main
#)

#include(GoogleTest)
#gtest_discover_tests(SimUDuck_tests)

#if (TARGET format-code)
#    add_dependencies(SimUDuck_tests format-code)
#endif ()