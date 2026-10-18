add_library(Editor_lib STATIC
        Model/Shapes/Shape.cpp
        Model/Shapes/Rectangle.cpp
        Model/Shapes/Ellipse.cpp
        Model/Shapes/Triangle.cpp
        Model/Document/Document.cpp
        Model/EditorState/EditorState.cpp
        Model/EditorState/SelectionFrame.cpp
        Model/Serialization/DocumentSerializer.cpp
        Command/AddShapeCommand/AddShapeCommand.cpp
        Command/DeleteShapeCommand/DeleteShapeCommand.cpp
        Command/ChangeShapeFrameCommand/ChangeShapeFrameCommand.cpp
        Controller/FrameCalculator.cpp
        Controller/ShapeDefaults.cpp
        Controller/EditorController.cpp
)

target_include_directories(Editor_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Editor
        main.cpp
        View/Canvas/Canvas.cpp
        View/Canvas/CanvasView.cpp
        View/Window/Toolbar.cpp
        View/Window/FileWindows.cpp
        View/Window/EditorWindow.cpp
)

target_include_directories(Editor PRIVATE ${SFML_INCLUDE_DIR})

target_link_libraries(Editor PRIVATE
        Editor_lib
        ${SFML_GRAPHICS_LIB}
        ${SFML_WINDOW_LIB}
        ${SFML_SYSTEM_LIB}
        ${COCOA_LIB}
        ${IOKIT_LIB}
        ${FOUNDATION_LIB}
        ${OPENGL_LIB}
)

add_executable(Editor_tests
        tests/ShapeTests.cpp
        tests/DocumentTests.cpp
        tests/SelectionFrameTests.cpp
        tests/CommandTests.cpp
        tests/FrameCalculatorTests.cpp
        tests/EditorControllerTests.cpp
        tests/DocumentSerializerTests.cpp
)

target_link_libraries(Editor_tests PRIVATE
        Editor_lib
        GTest::gtest
        GTest::gmock
        GTest::gtest_main
)

add_test(NAME Editor_tests COMMAND Editor_tests)
