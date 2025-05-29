#include "dataprovider.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QRandomGenerator>

DataProvider::DataProvider(QObject *parent)
    : QObject(parent)
    , m_isRunning(false)
    , m_useSimulatedData(true)  // 默认使用模拟数据（实际项目中应连接真实数据源）
{
    // 初始化模拟股票列表
    m_simulatedStocks = {
        {"600000", "浦发银行"},
        {"600036", "招商银行"},
        {"601398", "工商银行"},
        {"601988", "中国银行"},
        {"600519", "贵州茅台"},
        {"600887", "伊利股份"},
        {"000001", "平安银行"},
        {"000333", "美的集团"},
        {"000651", "格力电器"},
        {"000858", "五粮液"},
        {"300059", "东方财富"},
        {"300122", "智飞生物"},
        {"688111", "金山办公"},
        {"688981", "中芯国际"}
    };

    // 连接网络响应信号
    connect(&m_networkManager, &QNetworkAccessManager::finished,
            this, &DataProvider::onNetworkReply);
    
    // 连接模拟数据定时器
    connect(&m_simulateTimer, &QTimer::timeout,
            this, &DataProvider::onSimulateDataTimer);
}

DataProvider::~DataProvider()
{
    stop();
}

void DataProvider::start()
{
    if (!m_isRunning) {
        m_isRunning = true;
        
        if (m_useSimulatedData) {
            // 使用模拟数据，立即生成一次数据并启动定时器
            MarketData data = generateSimulatedData();
            emit dataReceived(data);
            
            // 启动模拟数据定时器（每3秒更新一次）
            m_simulateTimer.start(3000);
        } else {
            // 从真实数据源获取数据
            // TODO: 替换为实际的数据源URL
            fetchDataFromNetwork(QUrl("https://api.example.com/market/quotes"));
        }
    }
}

void DataProvider::stop()
{
    if (m_isRunning) {
        m_isRunning = false;
        
        // 停止模拟数据定时器
        if (m_simulateTimer.isActive()) {
            m_simulateTimer.stop();
        }
    }
}

void DataProvider::onRefreshRequested()
{
    if (m_isRunning) {
        if (m_useSimulatedData) {
            // 立即生成一次模拟数据
            MarketData data = generateSimulatedData();
            emit dataReceived(data);
        } else {
            // 从真实数据源获取数据
            fetchDataFromNetwork(QUrl("https://api.example.com/market/quotes"));
        }
    }
}

void DataProvider::onNetworkReply(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        // 读取数据
        QByteArray data = reply->readAll();
        
        // 解析数据
        MarketData marketData = parseMarketData(data);
        
        // 发送数据接收信号
        emit dataReceived(marketData);
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }
    
    reply->deleteLater();
}

void DataProvider::onSimulateDataTimer()
{
    if (m_isRunning && m_useSimulatedData) {
        // 生成模拟数据
        MarketData data = generateSimulatedData();
        
        // 发送数据接收信号
        emit dataReceived(data);
    }
}

void DataProvider::fetchDataFromNetwork(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    // 发送GET请求
    m_networkManager.get(request);
}

MarketData DataProvider::parseMarketData(const QByteArray& data)
{
    MarketData marketData;
    
    // 解析JSON数据
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isNull() && doc.isObject()) {
        QJsonObject root = doc.object();
        
        // 解析股票数据
        if (root.contains("stocks") && root["stocks"].isArray()) {
            QJsonArray stocks = root["stocks"].toArray();
            
            for (const QJsonValue& value : stocks) {
                if (value.isObject()) {
                    QJsonObject stock = value.toObject();
                    
                    // 创建股票对象
                    StockItem item;
                    
                    // 解析基本信息
                    if (stock.contains("code")) {
                        item.setCode(stock["code"].toString());
                    }
                    
                    if (stock.contains("name")) {
                        item.setName(stock["name"].toString());
                    }
                    
                    // 解析价格信息
                    if (stock.contains("current")) {
                        item.setCurrentPrice(stock["current"].toDouble());
                    }
                    
                    if (stock.contains("open")) {
                        item.setOpenPrice(stock["open"].toDouble());
                    }
                    
                    if (stock.contains("high")) {
                        item.setHighPrice(stock["high"].toDouble());
                    }
                    
                    if (stock.contains("low")) {
                        item.setLowPrice(stock["low"].toDouble());
                    }
                    
                    if (stock.contains("previous")) {
                        item.setPreviousClose(stock["previous"].toDouble());
                    }
                    
                    // 解析成交信息
                    if (stock.contains("volume")) {
                        item.setVolume(stock["volume"].toVariant().toLongLong());
                    }
                    
                    if (stock.contains("amount")) {
                        item.setAmount(stock["amount"].toDouble());
                    }
                    
                    // 设置更新时间
                    item.setUpdateTime(QDateTime::currentDateTime());
                    
                    // 添加到市场数据
                    marketData.addOrUpdateStock(item);
                }
            }
        }
        
        // 设置更新时间
        marketData.setUpdateTime(QDateTime::currentDateTime());
    }
    
    return marketData;
}

MarketData DataProvider::generateSimulatedData()
{
    MarketData marketData;
    QRandomGenerator *rng = QRandomGenerator::global();
    
    // 当前时间
    QDateTime now = QDateTime::currentDateTime();
    
    // 生成模拟股票数据
    for (auto it = m_simulatedStocks.constBegin(); it != m_simulatedStocks.constEnd(); ++it) {
        QString code = it.key();
        QString name = it.value();
        
        // 创建股票对象
        StockItem item(code, name);
        
        // 设置模拟价格（随机生成）
        double basePrice = 0.0;
        
        // 根据股票代码设置基础价格区间
        if (code.startsWith("600") || code.startsWith("601")) {
            basePrice = rng->bounded(10.0, 50.0);
        } else if (code.startsWith("000")) {
            basePrice = rng->bounded(8.0, 40.0);
        } else if (code.startsWith("300")) {
            basePrice = rng->bounded(30.0, 80.0);
        } else if (code.startsWith("688")) {
            basePrice = rng->bounded(50.0, 150.0);
        }
        
        // 昨收价
        double previousClose = basePrice * (1.0 + rng->bounded(-0.02, 0.02));
        item.setPreviousClose(previousClose);
        
        // 当前价（在昨收价基础上波动）
        double currentPrice = previousClose * (1.0 + rng->bounded(-0.1, 0.1));
        item.setCurrentPrice(currentPrice);
        
        // 开盘价
        double openPrice = previousClose * (1.0 + rng->bounded(-0.03, 0.03));
        item.setOpenPrice(openPrice);
        
        // 确保最高价和最低价合理
        double highPrice = qMax(currentPrice, openPrice) * (1.0 + rng->bounded(0.0, 0.05));
        double lowPrice = qMin(currentPrice, openPrice) * (1.0 - rng->bounded(0.0, 0.05));
        
        item.setHighPrice(highPrice);
        item.setLowPrice(lowPrice);
        
        // 成交量和金额
        long long volume = rng->bounded(100000LL, 10000000LL);
        double amount = volume * currentPrice;
        
        item.setVolume(volume);
        item.setAmount(amount);
        
        // 设置更新时间
        item.setUpdateTime(now);
        
        // 生成K线数据（假设过去30个交易日的数据）
        QVector<StockTradeData> kLineData;
        
        // 从30天前开始
        QDateTime startDate = now.addDays(-30);
        double lastClose = previousClose * 0.9;  // 初始价格
        
        for (int i = 0; i < 30; i++) {
            StockTradeData data;
            data.timestamp = startDate.addDays(i);
            
            // 基于前一天收盘价随机生成今天的价格
            double dailyChange = rng->bounded(-0.05, 0.05);
            data.open = lastClose * (1.0 + rng->bounded(-0.01, 0.01));
            data.close = lastClose * (1.0 + dailyChange);
            data.high = qMax(data.open, data.close) * (1.0 + rng->bounded(0.0, 0.03));
            data.low = qMin(data.open, data.close) * (1.0 - rng->bounded(0.0, 0.03));
            data.volume = rng->bounded(500000LL, 5000000LL);
            data.amount = data.volume * (data.high + data.low) / 2;
            
            kLineData.append(data);
            lastClose = data.close;
        }
        
        item.setKLineData(kLineData);
        
        // 生成分时数据（假设今天的数据）
        QVector<TimeSeriesPoint> timeSeriesData;
        
        // 交易时间9:30-11:30, 13:00-15:00
        QTime startTime1(9, 30);
        QTime endTime1(11, 30);
        QTime startTime2(13, 0);
        QTime endTime2(15, 0);
        
        QDateTime today = now.date().startOfDay();
        double lastPrice = openPrice;
        
        // 上午交易时段
        for (int minute = 0; minute < 120; minute++) {
            QTime time = startTime1.addSecs(minute * 60);
            QDateTime timestamp = today.addSecs(time.hour() * 3600 + time.minute() * 60);
            
            TimeSeriesPoint point;
            point.timestamp = timestamp;
            point.price = lastPrice * (1.0 + rng->bounded(-0.005, 0.005));
            point.volume = rng->bounded(10000LL, 100000LL);
            
            timeSeriesData.append(point);
            lastPrice = point.price;
        }
        
        // 下午交易时段
        for (int minute = 0; minute < 120; minute++) {
            QTime time = startTime2.addSecs(minute * 60);
            QDateTime timestamp = today.addSecs(time.hour() * 3600 + time.minute() * 60);
            
            TimeSeriesPoint point;
            point.timestamp = timestamp;
            point.price = lastPrice * (1.0 + rng->bounded(-0.005, 0.005));
            point.volume = rng->bounded(10000LL, 100000LL);
            
            timeSeriesData.append(point);
            lastPrice = point.price;
        }
        
        item.setTimeSeriesData(timeSeriesData);
        
        // 添加到市场数据
        marketData.addOrUpdateStock(item);
    }
    
    // 设置更新时间
    marketData.setUpdateTime(now);
    
    return marketData;
} 