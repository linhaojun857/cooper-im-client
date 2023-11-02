cmake_minimum_required(VERSION 3.22)
project(cooper_im_client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_PREFIX_PATH "D:/Qt6/6.5.2/msvc2019_64")

set(FRAMELESSHELPER_BUILD_STATIC ON)
set(FRAMELESSHELPER_NO_DEBUG_OUTPUT ON)
set(FRAMELESSHELPER_BUILD_QUICK OFF)
add_subdirectory(third_party/framelesshelper)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        Concurrent
        Core5Compat
        WebEngineWidgets
        WebChannel
        Sql
        REQUIRED)

add_library(cooper_im_lib
        # net
        net/INet.hpp
        net/TcpClientNet.hpp
        net/TcpClientNet.cpp
        # mediator
        mediator/INetMediator.hpp
        mediator/TcpClientMediator.hpp
        mediator/TcpClientMediator.cpp
        # entity
        entity/Entity.hpp
        entity/Entity.cpp
        # view
        view/LRWidget.hpp
        view/LRWidget.cpp
        view/LRWidget.ui
        view/MainWidget.hpp
        view/MainWidget.cpp
        view/MainWidget.ui
        view/MsgWidget.hpp
        view/MsgWidget.cpp
        view/MsgWidget.ui
        view/CotPWidget.hpp
        view/CotPWidget.cpp
        view/CotPWidget.ui
        view/PyqWidget.hpp
        view/PyqWidget.cpp
        view/PyqWidget.ui
        view/FriendWidget.hpp
        view/FriendWidget.cpp
        view/FriendWidget.ui
        view/GroupWidget.hpp
        view/GroupWidget.cpp
        view/GroupWidget.ui
        view/FriendItem.hpp
        view/FriendItem.cpp
        view/FriendItem.ui
        view/ChatDialog.hpp
        view/ChatDialog.cpp
        view/ChatDialog.ui
        view/ChatItem.hpp
        view/ChatItem.cpp
        view/ChatItem.ui
        view/ApplyNotifyItem.hpp
        view/ApplyNotifyItem.cpp
        view/ApplyNotifyItem.ui
        view/NotifyWidget.hpp
        view/NotifyWidget.cpp
        view/NotifyWidget.ui
        view/GroupItem.hpp
        view/GroupItem.cpp
        view/GroupItem.ui
        view/FGSWidget.hpp
        view/FGSWidget.cpp
        view/FGSWidget.ui
        view/FSRItem.hpp
        view/FSRItem.cpp
        view/FSRItem.ui
        view/AFDialog.hpp
        view/AFDialog.cpp
        view/AFDialog.ui
        view/ImageViewWidget.hpp
        view/ImageViewWidget.cpp
        view/ImageViewWidget.ui
        view/MessageWidget.hpp
        view/MessageWidget.cpp
        view/MessageWidget.ui
        view/MessageItem.hpp
        view/MessageItem.cpp
        view/MessageItem.ui
        view/CGWidget.hpp
        view/CGWidget.cpp
        view/CGWidget.ui
        view/GSRItem.hpp
        view/GSRItem.cpp
        view/GSRItem.ui
        # core
        core/IMKernel.hpp
        core/IMKernel.cpp
        # data
        data/DataSync.hpp
        data/DataSync.cpp
        # util
        util/HttpUtil.cpp
        util/HttpUtil.hpp
        # config
        define/IMDefine.hpp
        # mock
        mock/Mock.hpp
        mock/Mock.cpp
        # store
        store/IMStore.hpp
        store/IMStore.cpp)

target_include_directories(cooper_im_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(cooper_im_lib PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
        Qt::Concurrent
        Qt::Core5Compat
        Qt::WebEngineWidgets
        Qt::WebChannel
        Qt::Sql
        FramelessHelper::Core
        FramelessHelper::Widgets
)

add_executable(cooper_im_client
        main.cpp
        # qrc
        res/res.qrc)
target_link_libraries(cooper_im_client PRIVATE cooper_im_lib)

add_executable(cooper_im_client_1
        main.cpp
        # qrc
        res/res.qrc)
target_link_libraries(cooper_im_client_1 PRIVATE cooper_im_lib)

add_executable(cooper_im_client_2
        main.cpp
        # qrc
        res/res.qrc)
target_link_libraries(cooper_im_client_2 PRIVATE cooper_im_lib)

file(GLOB_RECURSE TEST_SOURCES "test/*.cpp")
foreach (TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} res/res.qrc)
    target_link_libraries(${TEST_NAME} PRIVATE cooper_im_lib)
endforeach ()

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
