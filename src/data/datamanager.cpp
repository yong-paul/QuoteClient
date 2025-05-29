#include "datamanager.h"
#include <QDebug>

DataManager::DataManager(QObject *parent)
    : QObject(parent)
    , m_refreshInterval(5000)  // 默认5秒刷新一次
{
    // 设置自动刷新定时器
    connect(&m_autoRefreshTimer, &QTimer::timeout,
            this, &DataManager::onAutoRefreshTimer);
}

DataManager::~DataManager()
{
    stopAutoRefresh();
}

const MarketData& DataManager::getMarketData() const
{
    return m_marketData;
}

const StockItem* DataManager::getStock(const QString& code) const
{
    return m_marketData.getStock(code);
}

QStringList DataManager::getStocksByMarketType(StockItem::MarketType type) const
{
    return m_marketData.getStocksByMarketType(type);
}

void DataManager::setRefreshInterval(int msecs)
{
    if (msecs > 0) {
        m_refreshInterval = msecs;
        
        // 如果定时器正在运行，则重启
        if (m_autoRefreshTimer.isActive()) {
            m_autoRefreshTimer.stop();
            m_autoRefreshTimer.start(m_refreshInterval);
        }
    }
}

void DataManager::startAutoRefresh()
{
    if (!m_autoRefreshTimer.isActive()) {
        m_autoRefreshTimer.start(m_refreshInterval);
    }
}

void DataManager::stopAutoRefresh()
{
    if (m_autoRefreshTimer.isActive()) {
        m_autoRefreshTimer.stop();
    }
}

void DataManager::updateMarketData(const MarketData& data)
{
    // 更新数据
    m_marketData = data;
    
    // 发送数据更新信号
    emit marketDataUpdated(m_marketData);
}

void DataManager::requestRefresh()
{
    // 发送刷新请求信号
    emit refreshRequested();
}

void DataManager::onAutoRefreshTimer()
{
    // 定时器触发时请求刷新数据
    requestRefresh();
} 