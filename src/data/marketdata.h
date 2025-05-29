#pragma once

#include "stockitem.h"
#include <QMap>
#include <QDateTime>
#include <QStringList>

/**
 * @brief 市场数据类
 * 
 * 负责管理和存储所有股票数据
 */
class MarketData
{
public:
    MarketData();
    ~MarketData();
    
    /**
     * @brief 获取指定代码的股票
     * @param code 股票代码
     * @return 股票对象指针，如果不存在则返回nullptr
     */
    const StockItem* getStock(const QString& code) const;
    
    /**
     * @brief 添加或更新股票
     * @param stock 股票对象
     */
    void addOrUpdateStock(const StockItem& stock);
    
    /**
     * @brief 移除股票
     * @param code 股票代码
     */
    void removeStock(const QString& code);
    
    /**
     * @brief 获取所有股票代码
     * @return 股票代码列表
     */
    QStringList getAllStockCodes() const;
    
    /**
     * @brief 获取所有股票
     * @return 股票映射表
     */
    const QMap<QString, StockItem>& getAllStocks() const;
    
    /**
     * @brief 获取指定市场类型的股票代码
     * @param type 市场类型
     * @return 股票代码列表
     */
    QStringList getStocksByMarketType(StockItem::MarketType type) const;
    
    /**
     * @brief 清空所有数据
     */
    void clear();
    
    /**
     * @brief 获取更新时间
     * @return 最后更新时间
     */
    QDateTime getUpdateTime() const;
    
    /**
     * @brief 设置更新时间
     * @param time 更新时间
     */
    void setUpdateTime(const QDateTime& time);
    
private:
    QMap<QString, StockItem> m_stocks;  // 股票映射表 (代码 -> 股票对象)
    QDateTime m_updateTime;             // 最后更新时间
}; 