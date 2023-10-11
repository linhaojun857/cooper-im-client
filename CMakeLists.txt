cmake_minimum_required(VERSION 3.22)
project(cooper_im_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "D:/Qt6/6.5.2/mingw_64")


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        Core5Compat
        REQUIRED)

add_executable(cooper_im_client
        main.cpp
        # net
        net/INet.hpp
        net/TcpClientNet.hpp
        net/TcpClientNet.cpp
        # mediator
        mediator/INetMediator.hpp
        mediator/TcpClientMediator.hpp
        mediator/TcpClientMediator.cpp
        # view
        view/LRWidget.hpp
        view/LRWidget.cpp
        view/LRWidget.ui
        view/MainWidget.hpp
        view/MainWidget.cpp
        view/MainWidget.ui
        # core
        core/IMKernel.hpp
        core/IMKernel.cpp
        # util
        util/HttpUtil.cpp
        util/HttpUtil.hpp
        # config
        config/IMDefine.hpp
        # qrc
        res/res.qrc)

target_include_directories(cooper_im_client PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cooper_im_client
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Core5Compat
)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
