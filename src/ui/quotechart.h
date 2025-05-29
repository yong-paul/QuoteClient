#pragma once

#include "../data/stockitem.h"
#include <QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QCandlestickSeries>
#include <QtCharts/QCandlestickSet>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>

QT_CHARTS_USE_NAMESPACE

/**
 * @brief 行情图表类
 * 
 * 用于显示分时图和K线图
 */
class QuoteChart : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief 图表类型枚举
     */
    enum class ChartType {
        TimeSeries,    // 分时图
        Candlestick    // K线图
    };
    
    /**
     * @brief 周期类型枚举
     */
    enum class PeriodType {
        Day,           // 日K
        Week,          // 周K
        Month,         // 月K
        Minutes,       // 分钟K
        Minutes5,      // 5分钟K
        Minutes15,     // 15分钟K
        Minutes30,     // 30分钟K
        Minutes60      // 60分钟K
    };

public:
    explicit QuoteChart(QWidget *parent = nullptr);
    ~QuoteChart();
    
    /**
     * @brief 更新图表
     * @param stock 股票数据
     */
    void updateChart(const StockItem& stock);
    
    /**
     * @brief 设置图表类型
     * @param type 图表类型
     */
    void setChartType(ChartType type);
    
    /**
     * @brief 设置周期类型
     * @param type 周期类型
     */
    void setPeriodType(PeriodType type);

signals:
    /**
     * @brief 股票变化信号
     * @param code 股票代码
     */
    void stockChanged(const QString& code);

private slots:
    /**
     * @brief 处理周期选择变化
     * @param index 索引
     */
    void onPeriodChanged(int index);
    
    /**
     * @brief 处理图表类型按钮点击
     */
    void onTimeSeriesButtonClicked();
    void onCandlestickButtonClicked();

private:
    /**
     * @brief 创建UI
     */
    void setupUI();
    
    /**
     * @brief 创建分时图
     * @param stock 股票数据
     */
    void createTimeSeriesChart(const StockItem& stock);
    
    /**
     * @brief 创建K线图
     * @param stock 股票数据
     */
    void createCandlestickChart(const StockItem& stock);
    
    /**
     * @brief 清除图表
     */
    void clearChart();

private:
    QChartView *m_chartView;           // 图表视图
    QChart *m_chart;                    // 图表
    
    // 分时图相关
    QLineSeries *m_priceSeries;         // 价格线
    QBarSeries *m_volumeSeries;         // 成交量柱状图
    
    // K线图相关
    QCandlestickSeries *m_candleSeries; // K线图系列
    QBarSeries *m_candleVolumeSeries;   // K线成交量柱状图
    
    // 坐标轴
    QDateTimeAxis *m_timeAxis;          // 时间轴
    QValueAxis *m_priceAxis;            // 价格轴
    QValueAxis *m_volumeAxis;           // 成交量轴
    
    // 控制组件
    QComboBox *m_periodComboBox;        // 周期选择
    QPushButton *m_timeSeriesButton;    // 分时图按钮
    QPushButton *m_candlestickButton;   // K线图按钮
    QLabel *m_infoLabel;                // 信息标签
    
    // 当前状态
    ChartType m_chartType;              // 当前图表类型
    PeriodType m_periodType;            // 当前周期类型
    QString m_currentStockCode;         // 当前股票代码
}; 