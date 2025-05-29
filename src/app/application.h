#pragma once

#include "mainwindow.h"
#include "../data/datamanager.h"
#include "../network/dataprovider.h"

#include <QObject>
#include <memory>

/**
 * @brief 应用程序类，负责协调各个组件
 * 
 * 这个类负责初始化和管理应用程序的主要组件，实现了UI和数据层的解耦
 */
class Application : public QObject
{
    Q_OBJECT

public:
    explicit Application(QObject *parent = nullptr);
    ~Application();

    /**
     * @brief 初始化应用程序
     * 
     * 创建并初始化所有必要的组件
     */
    void initialize();

private:
    // UI组件
    std::unique_ptr<MainWindow> m_mainWindow;
    
    // 数据管理
    std::unique_ptr<DataManager> m_dataManager;
    
    // 网络数据提供者
    std::unique_ptr<DataProvider> m_dataProvider;
}; 