cmake_minimum_required(VERSION 3.26)
project(chess)

if(WIN32)
    # For Windows
elseif(UNIX)
    # For Linux
endif()

find_package(SFML COMPONENTS graphics REQUIRED)

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

target_link_libraries(${PROJECT_NAME} sfml-graphics)