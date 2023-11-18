cmake_minimum_required(VERSION 3.26)
project(chess)

if(WIN32)
    # For Windows
    set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML_WIN)
elseif(UNIX)
    # For Linux
    set(SFML_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib/SFML_UNIX)
endif()

include_directories(
        ${SFML_DIR}/include
)

link_directories(${SFML_DIR}/lib)

set(SFML_STATIC_LIBRARIES TRUE)
add_compile_definitions(SFML_STATIC)

set(CMAKE_CXX_STANDARD 20)

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

target_link_libraries(chess PRIVATE sfml-graphics sfml-window sfml-system)