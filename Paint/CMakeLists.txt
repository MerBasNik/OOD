add_executable(Paint
        main.cpp
        Shape/Shape.cpp
        Picture/Picture.cpp
        Lib/Parser/Parser.cpp
        Lib/Parser/ParseBehavior.cpp
        Canvas/Canvas.cpp
        Behavior/CircleBehavior.cpp
        Behavior/LineBehavior.cpp
        Behavior/RectangleBehavior.cpp
        Behavior/TriangleBehavior.cpp
        Behavior/TextBehavior.cpp
        Command/AddShapeCommand/AddShapeCommand.cpp
        Command/ChangeColorCommand/ChangeColorCommand.cpp
        Command/DrawShapeCommand/DrawShapeCommand.cpp
        Command/CloneShapeCommand/CloneShapeCommand.cpp
        Command/ChangeShapeCommand/ChangeShapeCommand.cpp
        Command/DeleteShapeCommand/DeleteShapeCommand.cpp
        Command/MoveShapeCommand/MoveShapeCommand.cpp
        Command/MovePictureCommand/MovePictureCommand.cpp
        Command/ListCommand/ListCommand.cpp
        Command/DrawPictureCommand/DrawPictureCommand.cpp
)

target_include_directories(Paint PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Behavior
        ${CMAKE_CURRENT_SOURCE_DIR}/Canvas
        ${CMAKE_CURRENT_SOURCE_DIR}/Lib
        ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Color
        ${CMAKE_CURRENT_SOURCE_DIR}/Lib/Parser
        ${CMAKE_CURRENT_SOURCE_DIR}/Shape
        ${CMAKE_CURRENT_SOURCE_DIR}/Picture
        ${CMAKE_CURRENT_SOURCE_DIR}/Command
        ${SFML_INCLUDE_DIR}
)

target_link_libraries(Paint PRIVATE
        ${SFML_GRAPHICS_LIB}
        ${SFML_WINDOW_LIB}
        ${SFML_SYSTEM_LIB}
        ${COCOA_LIB}
        ${IOKIT_LIB}
        ${FOUNDATION_LIB}
        ${OPENGL_LIB}
)