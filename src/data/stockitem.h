#pragma once

#include <QString>
#include <QDateTime>
#include <QVector>

/**
 * @brief 股票交易数据结构体
 */
struct StockTradeData {
    QDateTime timestamp;  // 时间戳
    double open;          // 开盘价
    double high;          // 最高价
    double low;           // 最低价
    double close;         // 收盘价
    long long volume;     // 成交量
    double amount;        // 成交金额
    
    StockTradeData() : open(0), high(0), low(0), close(0), volume(0), amount(0) {}
};

/**
 * @brief 分时数据点结构体
 */
struct TimeSeriesPoint {
    QDateTime timestamp;  // 时间戳
    double price;         // 价格
    long long volume;     // 成交量
    
    TimeSeriesPoint() : price(0), volume(0) {}
};

/**
 * @brief 股票项类
 * 
 * 表示一支股票的所有相关数据
 */
class StockItem
{
public:
    // 股票市场类型
    enum class MarketType {
        Unknown,
        ShanghaiA,  // 上证A股
        ShenzhenA,  // 深证A股
        ChiNext,    // 创业板
        StarMarket  // 科创板
    };
    
public:
    StockItem();
    StockItem(const QString& code, const QString& name);
    ~StockItem();
    
    // 基本信息
    QString getCode() const { return m_code; }
    QString getName() const { return m_name; }
    MarketType getMarketType() const { return m_marketType; }
    
    void setCode(const QString& code) { m_code = code; }
    void setName(const QString& name) { m_name = name; }
    void setMarketType(MarketType type) { m_marketType = type; }
    
    // 当前价格信息
    double getCurrentPrice() const { return m_currentPrice; }
    double getOpenPrice() const { return m_openPrice; }
    double getHighPrice() const { return m_highPrice; }
    double getLowPrice() const { return m_lowPrice; }
    double getPreviousClose() const { return m_previousClose; }
    
    void setCurrentPrice(double price) { m_currentPrice = price; }
    void setOpenPrice(double price) { m_openPrice = price; }
    void setHighPrice(double price) { m_highPrice = price; }
    void setLowPrice(double price) { m_lowPrice = price; }
    void setPreviousClose(double price) { m_previousClose = price; }
    
    // 成交信息
    long long getVolume() const { return m_volume; }
    double getAmount() const { return m_amount; }
    
    void setVolume(long long volume) { m_volume = volume; }
    void setAmount(double amount) { m_amount = amount; }
    
    // 涨跌信息
    double getChange() const;
    double getChangePercent() const;
    
    // 历史K线数据
    const QVector<StockTradeData>& getKLineData() const { return m_kLineData; }
    void addKLineData(const StockTradeData& data) { m_kLineData.append(data); }
    void setKLineData(const QVector<StockTradeData>& data) { m_kLineData = data; }
    
    // 分时数据
    const QVector<TimeSeriesPoint>& getTimeSeriesData() const { return m_timeSeriesData; }
    void addTimeSeriesPoint(const TimeSeriesPoint& point) { m_timeSeriesData.append(point); }
    void setTimeSeriesData(const QVector<TimeSeriesPoint>& data) { m_timeSeriesData = data; }
    
    // 更新时间
    QDateTime getUpdateTime() const { return m_updateTime; }
    void setUpdateTime(const QDateTime& time) { m_updateTime = time; }
    
private:
    // 股票基本信息
    QString m_code;                          // 股票代码
    QString m_name;                          // 股票名称
    MarketType m_marketType;                 // 市场类型
    
    // 当前价格信息
    double m_currentPrice;                   // 当前价格
    double m_openPrice;                      // 开盘价
    double m_highPrice;                      // 最高价
    double m_lowPrice;                       // 最低价
    double m_previousClose;                  // 昨日收盘价
    
    // 成交信息
    long long m_volume;                      // 成交量
    double m_amount;                         // 成交金额
    
    // K线数据
    QVector<StockTradeData> m_kLineData;     // K线历史数据
    
    // 分时数据
    QVector<TimeSeriesPoint> m_timeSeriesData; // 分时数据
    
    // 更新时间
    QDateTime m_updateTime;                  // 数据更新时间
}; 