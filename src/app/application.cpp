#include "application.h"

Application::Application(QObject *parent)
    : QObject(parent)
{
}

Application::~Application()
{
}

void Application::initialize()
{
    // 创建数据提供者
    m_dataProvider = std::make_unique<DataProvider>();
    
    // 创建数据管理器
    m_dataManager = std::make_unique<DataManager>();
    
    // 连接数据提供者和数据管理器
    connect(m_dataProvider.get(), &DataProvider::dataReceived,
            m_dataManager.get(), &DataManager::updateMarketData);
    
    // 创建主窗口
    m_mainWindow = std::make_unique<MainWindow>();
    
    // 连接数据管理器和UI
    connect(m_dataManager.get(), &DataManager::marketDataUpdated,
            m_mainWindow.get(), &MainWindow::updateUI);
    
    // 启动数据提供者
    m_dataProvider->start();
    
    // 显示主窗口
    m_mainWindow->show();
} 