#pragma once

#include "../data/marketdata.h"
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QTimer>
#include <QMap>

/**
 * @brief 数据提供者类
 * 
 * 负责从网络或本地获取行情数据
 */
class DataProvider : public QObject
{
    Q_OBJECT

public:
    explicit DataProvider(QObject *parent = nullptr);
    ~DataProvider();

    /**
     * @brief 启动数据提供者
     */
    void start();

    /**
     * @brief 停止数据提供者
     */
    void stop();

signals:
    /**
     * @brief 数据接收完成信号
     * @param data 接收到的市场数据
     */
    void dataReceived(const MarketData& data);

public slots:
    /**
     * @brief 处理刷新请求
     */
    void onRefreshRequested();

private slots:
    /**
     * @brief 处理网络响应
     * @param reply 网络响应对象
     */
    void onNetworkReply(QNetworkReply* reply);

    /**
     * @brief 处理模拟数据生成定时器
     */
    void onSimulateDataTimer();

private:
    /**
     * @brief 从网络获取数据
     * @param url 数据URL
     */
    void fetchDataFromNetwork(const QUrl& url);

    /**
     * @brief 解析行情数据
     * @param data 原始数据
     * @return 解析后的市场数据
     */
    MarketData parseMarketData(const QByteArray& data);

    /**
     * @brief 生成模拟数据（开发测试用）
     * @return 模拟的市场数据
     */
    MarketData generateSimulatedData();

private:
    QNetworkAccessManager m_networkManager;  // 网络管理器
    QTimer m_simulateTimer;                  // 模拟数据定时器
    bool m_isRunning;                        // 运行状态标志
    bool m_useSimulatedData;                 // 是否使用模拟数据

    // 预设股票列表（用于模拟数据）
    QMap<QString, QString> m_simulatedStocks;
}; 