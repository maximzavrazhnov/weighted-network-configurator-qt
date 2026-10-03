cmake_minimum_required(VERSION 3.21)

project(weighted_network_configurator
    VERSION 1.0.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(network_core
    src/core/player.cpp
    src/core/configuration.cpp
    src/core/system.cpp
)

target_include_directories(network_core PUBLIC src/core)

if(MSVC)
    target_compile_options(network_core PRIVATE /W4)
else()
    target_compile_options(network_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

add_executable(core_smoke_test
    tests/core_smoke.cpp
)

target_link_libraries(core_smoke_test PRIVATE network_core)
add_test(NAME core_smoke_test COMMAND core_smoke_test)

option(BUILD_QT_GUI "Build the Qt desktop application" ON)

if(BUILD_QT_GUI)
    find_package(Qt6 COMPONENTS Widgets QUIET)

    if(Qt6_FOUND)
        set(CMAKE_AUTOMOC ON)
        set(CMAKE_AUTOUIC ON)
        set(CMAKE_AUTORCC ON)

        add_executable(network_configurator
            src/gui/main.cpp
            src/gui/Players.cpp
            src/gui/Players.h
            src/gui/Players.ui
            src/gui/Players.qrc
        )

        target_include_directories(network_configurator PRIVATE
            src/core
            src/gui
        )

        target_link_libraries(network_configurator PRIVATE
            network_core
            Qt6::Widgets
        )

        if(MSVC)
            target_compile_options(network_configurator PRIVATE /W4)
        else()
            target_compile_options(network_configurator PRIVATE -Wall -Wextra -Wpedantic)
        endif()
    else()
        message(WARNING "Qt6 Widgets not found: building core and tests only.")
    endif()
endif()
