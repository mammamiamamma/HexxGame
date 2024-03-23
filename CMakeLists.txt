cmake_minimum_required(VERSION 3.24)
project(HexxGame)
set(CMAKE_CXX_STANDARD 23)
set(BUILD_SHARED_LIBS FALSE)#!!

include(FetchContent)
FETCHCONTENT_DECLARE(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
)

FETCHCONTENT_MAKEAVAILABLE(SFML)

add_executable(HexxGame source/main.cpp source/HexBoard.cpp source/HexBoard.h source/Player.cpp source/Player.h source/StoneHelper.cpp source/StoneHelper.h source/Bot.cpp source/Bot.h source/Application.cpp source/Application.h source/Game.cpp source/Game.h source/Menu.cpp source/Menu.h source/ui/Button.cpp source/ui/Button.h source/ui/ButtonGenerator.cpp source/ui/ButtonGenerator.h source/MenuHandler.cpp source/MenuHandler.h)

target_link_libraries(HexxGame PRIVATE sfml-system sfml-window
        sfml-graphics)
