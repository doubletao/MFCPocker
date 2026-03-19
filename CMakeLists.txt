cmake_minimum_required(VERSION 3.16)
project(MFCPoker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# 添加当前目录到包含路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 REQUIRED COMPONENTS Core Widgets LinguistTools)

# 收集所有源文件
set(SOURCES
    main.cpp
    ui/MainWindow.cpp
    ui/GameWidget.cpp
    ui/GameButton.cpp
    core/interfaces/ICard.cpp
    core/interfaces/IClickable.cpp
    core/interfaces/IDrawable.cpp
    core/interfaces/ISortable.cpp
    core/models/DataCenter.cpp
    core/models/CardGroup.cpp
    core/models/CardPile.cpp
    core/models/Judge.cpp
    core/models/Player.cpp
    core/models/PokerCard.cpp
    core/ai/RobotPilot.cpp
    core/utils/GlobalUtils.cpp
)

set(HEADERS
    ui/MainWindow.h
    ui/GameWidget.h
    ui/GameButton.h
    core/interfaces/ICard.h
    core/interfaces/IClickable.h
    core/interfaces/IDrawable.h
    core/interfaces/ISortable.h
    core/models/DataCenter.h
    core/models/CardGroup.h
    core/models/CardPile.h
    core/models/Judge.h
    core/models/Player.h
    core/models/PokerCard.h
    core/ai/RobotPilot.h
    core/utils/GlobalUtils.h
)

# 国际化处理
set(TS_FILES
    i18n/MFCPoker_zh_CN.ts
)

qt5_add_translation(QM_FILES ${TS_FILES})

add_executable(MFCPoker
    ${SOURCES}
    ${HEADERS}
    ${QM_FILES}
    resources.qrc
)

target_link_libraries(MFCPoker PRIVATE Qt5::Core Qt5::Widgets)

if(MSVC)
    target_compile_options(MFCPoker PRIVATE /utf-8)
endif()
