cmake_minimum_required(VERSION 3.26)
project(chess)


find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

set(CMAKE_CXX_STANDARD 20)
add_executable(chess main.cpp
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
)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-audio)
