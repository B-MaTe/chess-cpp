cmake_minimum_required(VERSION 3.25)
project(chess)

set(SOURCES
        main.cpp
        texture/CustomTexture.cpp
        texture/CustomTexture.hpp
        board/Board.cpp
        board/Board.hpp
        piece/Piece.cpp
        piece/Piece.hpp
        helper/FenHelper.cpp
        helper/FenHelper.hpp
        game/Game.cpp
        game/Game.hpp
        enums/Status.cpp
        prompt/PiecePromotionPrompt.cpp
        prompt/PiecePromotionPrompt.hpp
        globals.hpp
)

add_executable(chess ${SOURCES})

if(UNIX)
    # For Linux
    find_package(SFML COMPONENTS graphics REQUIRED)
    target_link_libraries(${PROJECT_NAME} sfml-graphics)
endif()

