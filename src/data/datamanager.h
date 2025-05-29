#pragma once

#include "marketdata.h"
#include <QObject>
#include <QTimer>
#include <memory>

/**
 * @brief 数据管理器类
 * 
 * 负责管理市场数据，处理数据更新和缓存
 */
class DataManager : public QObject
{
    Q_OBJECT

public:
    explicit DataManager(QObject *parent = nullptr);
    ~DataManager();

    /**
     * @brief 获取当前的市场数据
     * @return 市场数据对象的引用
     */
    const MarketData& getMarketData() const;

    /**
     * @brief 获取指定代码的股票
     * @param code 股票代码
     * @return 股票对象指针，如果不存在则返回nullptr
     */
    const StockItem* getStock(const QString& code) const;

    /**
     * @brief 获取指定市场类型的股票列表
     * @param type 市场类型
     * @return 股票代码列表
     */
    QStringList getStocksByMarketType(StockItem::MarketType type) const;

    /**
     * @brief 设置自动刷新间隔
     * @param msecs 刷新间隔（毫秒）
     */
    void setRefreshInterval(int msecs);

    /**
     * @brief 启动自动刷新
     */
    void startAutoRefresh();

    /**
     * @brief 停止自动刷新
     */
    void stopAutoRefresh();

public slots:
    /**
     * @brief 更新市场数据
     * @param data 新的市场数据
     */
    void updateMarketData(const MarketData& data);

    /**
     * @brief 请求刷新数据
     */
    void requestRefresh();

signals:
    /**
     * @brief 市场数据已更新信号
     * @param data 更新后的市场数据
     */
    void marketDataUpdated(const MarketData& data);

    /**
     * @brief 请求数据提供者刷新数据信号
     */
    void refreshRequested();

private slots:
    /**
     * @brief 自动刷新定时器触发
     */
    void onAutoRefreshTimer();

private:
    MarketData m_marketData;        // 市场数据
    QTimer m_autoRefreshTimer;      // 自动刷新定时器
    int m_refreshInterval;          // 刷新间隔（毫秒）
}; 