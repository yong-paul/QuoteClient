add_executable(QuoteClient
    main.cpp
    app/application.cpp
    app/application.h
    app/mainwindow.cpp
    app/mainwindow.h
    app/mainwindow.ui
    data/marketdata.cpp
    data/marketdata.h
    data/stockitem.cpp
    data/stockitem.h
    data/datamanager.cpp
    data/datamanager.h
    network/dataprovider.cpp
    network/dataprovider.h
    ui/stocktable.cpp
    ui/stocktable.h
    ui/quotechart.cpp
    ui/quotechart.h
    resources/resources.qrc
)

target_include_directories(QuoteClient PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(QuoteClient PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Network
    Qt6::Charts
)

# 安装配置
install(TARGETS QuoteClient
    RUNTIME DESTINATION bin
) 