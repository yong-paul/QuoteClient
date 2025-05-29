#include "stockitem.h"

StockItem::StockItem()
    : m_marketType(MarketType::Unknown)
    , m_currentPrice(0.0)
    , m_openPrice(0.0)
    , m_highPrice(0.0)
    , m_lowPrice(0.0)
    , m_previousClose(0.0)
    , m_volume(0)
    , m_amount(0.0)
{
}

StockItem::StockItem(const QString& code, const QString& name)
    : m_code(code)
    , m_name(name)
    , m_marketType(MarketType::Unknown)
    , m_currentPrice(0.0)
    , m_openPrice(0.0)
    , m_highPrice(0.0)
    , m_lowPrice(0.0)
    , m_previousClose(0.0)
    , m_volume(0)
    , m_amount(0.0)
{
    // 根据股票代码判断市场类型
    if (code.startsWith("60")) {
        m_marketType = MarketType::ShanghaiA;
    } else if (code.startsWith("00")) {
        m_marketType = MarketType::ShenzhenA;
    } else if (code.startsWith("30")) {
        m_marketType = MarketType::ChiNext;
    } else if (code.startsWith("68")) {
        m_marketType = MarketType::StarMarket;
    }
}

StockItem::~StockItem()
{
}

double StockItem::getChange() const
{
    return m_currentPrice - m_previousClose;
}

double StockItem::getChangePercent() const
{
    if (m_previousClose <= 0.0) {
        return 0.0;
    }
    
    return (m_currentPrice - m_previousClose) / m_previousClose * 100.0;
} 