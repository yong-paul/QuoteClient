#include "marketdata.h"

MarketData::MarketData()
{
}

MarketData::~MarketData()
{
}

const StockItem* MarketData::getStock(const QString& code) const
{
    if (m_stocks.contains(code)) {
        return &m_stocks[code];
    }
    
    return nullptr;
}

void MarketData::addOrUpdateStock(const StockItem& stock)
{
    m_stocks[stock.getCode()] = stock;
}

void MarketData::removeStock(const QString& code)
{
    m_stocks.remove(code);
}

QStringList MarketData::getAllStockCodes() const
{
    return m_stocks.keys();
}

const QMap<QString, StockItem>& MarketData::getAllStocks() const
{
    return m_stocks;
}

QStringList MarketData::getStocksByMarketType(StockItem::MarketType type) const
{
    QStringList codes;
    
    for (auto it = m_stocks.cbegin(); it != m_stocks.cend(); ++it) {
        if (it.value().getMarketType() == type) {
            codes.append(it.key());
        }
    }
    
    return codes;
}

void MarketData::clear()
{
    m_stocks.clear();
}

QDateTime MarketData::getUpdateTime() const
{
    return m_updateTime;
}

void MarketData::setUpdateTime(const QDateTime& time)
{
    m_updateTime = time;
} 