#include "quotechart.h"
#include <QDateTime>
#include <QDebug>
#include <QGridLayout>
#include <QSpacerItem>

QuoteChart::QuoteChart(QWidget *parent)
    : QWidget(parent)
    , m_chartView(nullptr)
    , m_chart(nullptr)
    , m_priceSeries(nullptr)
    , m_volumeSeries(nullptr)
    , m_candleSeries(nullptr)
    , m_candleVolumeSeries(nullptr)
    , m_timeAxis(nullptr)
    , m_priceAxis(nullptr)
    , m_volumeAxis(nullptr)
    , m_periodComboBox(nullptr)
    , m_timeSeriesButton(nullptr)
    , m_candlestickButton(nullptr)
    , m_infoLabel(nullptr)
    , m_chartType(ChartType::TimeSeries)
    , m_periodType(PeriodType::Day)
    , m_loadingLabel(nullptr)
{
    setupUI();
}

QuoteChart::~QuoteChart()
{
    clearChart();
}

void QuoteChart::updateChart(const StockItem& stock)
{
    m_currentStockCode = stock.getCode();
    
    // 更新股票信息标签
    QString infoText = QString("%1 (%2) %3 %4 (%5%)")
                        .arg(stock.getName())
                        .arg(stock.getCode())
                        .arg(QString::number(stock.getCurrentPrice(), 'f', 2))
                        .arg(QString::number(stock.getChange(), 'f', 2))
                        .arg(QString::number(stock.getChangePercent(), 'f', 2));
    
    // 设置颜色
    if (stock.getChangePercent() > 0) {
        m_infoLabel->setStyleSheet("color: red;");
    } else if (stock.getChangePercent() < 0) {
        m_infoLabel->setStyleSheet("color: green;");
    } else {
        m_infoLabel->setStyleSheet("color: black;");
    }
    
    m_infoLabel->setText(infoText);
    
    // 根据当前图表类型更新图表
    switch (m_chartType) {
    case ChartType::TimeSeries:
        createTimeSeriesChart(stock);
        break;
    case ChartType::Candlestick:
        createCandlestickChart(stock);
        break;
    }
}

void QuoteChart::setChartType(ChartType type)
{
    if (m_chartType != type) {
        m_chartType = type;
        
        // 更新按钮状态
        m_timeSeriesButton->setChecked(type == ChartType::TimeSeries);
        m_candlestickButton->setChecked(type == ChartType::Candlestick);
        
        // 如果有当前股票，则更新图表
        if (!m_currentStockCode.isEmpty()) {
            // 清除当前图表
            clearChart();
            
            // 重新创建图表（这里需要获取最新的股票数据）
            // 在实际应用中，应该通过数据管理器获取股票数据
            // 此处简化处理，通过信号通知需要更新
            emit stockChanged(m_currentStockCode);
        }
    }
}

void QuoteChart::setPeriodType(PeriodType type)
{
    if (m_periodType != type) {
        m_periodType = type;
        
        // 如果是K线图且有当前股票，则更新图表
        if (m_chartType == ChartType::Candlestick && !m_currentStockCode.isEmpty()) {
            emit stockChanged(m_currentStockCode);
        }
    }
}

void QuoteChart::onPeriodChanged(int index)
{
    // 根据索引设置周期类型
    switch (index) {
    case 0:
        setPeriodType(PeriodType::Day);
        break;
    case 1:
        setPeriodType(PeriodType::Week);
        break;
    case 2:
        setPeriodType(PeriodType::Month);
        break;
    case 3:
        setPeriodType(PeriodType::Minutes);
        break;
    case 4:
        setPeriodType(PeriodType::Minutes5);
        break;
    case 5:
        setPeriodType(PeriodType::Minutes15);
        break;
    case 6:
        setPeriodType(PeriodType::Minutes30);
        break;
    case 7:
        setPeriodType(PeriodType::Minutes60);
        break;
    }
}

void QuoteChart::onTimeSeriesButtonClicked()
{
    setChartType(ChartType::TimeSeries);
}

void QuoteChart::onCandlestickButtonClicked()
{
    setChartType(ChartType::Candlestick);
}

void QuoteChart::setupUI()
{
    // 创建主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // 创建控制面板
    QWidget *controlPanel = new QWidget();
    QHBoxLayout *controlLayout = new QHBoxLayout(controlPanel);
    controlLayout->setContentsMargins(0, 0, 0, 0);
    
    // 添加图表类型按钮
    m_timeSeriesButton = new QPushButton(tr("分时图"));
    m_timeSeriesButton->setCheckable(true);
    m_timeSeriesButton->setChecked(true);
    
    m_candlestickButton = new QPushButton(tr("K线图"));
    m_candlestickButton->setCheckable(true);
    
    // 添加周期选择下拉框
    QLabel *periodLabel = new QLabel(tr("周期:"));
    m_periodComboBox = new QComboBox();
    m_periodComboBox->addItem(tr("日K"));
    m_periodComboBox->addItem(tr("周K"));
    m_periodComboBox->addItem(tr("月K"));
    m_periodComboBox->addItem(tr("分钟"));
    m_periodComboBox->addItem(tr("5分钟"));
    m_periodComboBox->addItem(tr("15分钟"));
    m_periodComboBox->addItem(tr("30分钟"));
    m_periodComboBox->addItem(tr("60分钟"));
    
    // 添加股票信息标签
    m_infoLabel = new QLabel();
    QFont font = m_infoLabel->font();
    font.setBold(true);
    font.setPointSize(11);
    m_infoLabel->setFont(font);
    
    // 添加控件到控制布局
    controlLayout->addWidget(m_timeSeriesButton);
    controlLayout->addWidget(m_candlestickButton);
    controlLayout->addSpacing(10);
    controlLayout->addWidget(periodLabel);
    controlLayout->addWidget(m_periodComboBox);
    controlLayout->addStretch();
    controlLayout->addWidget(m_infoLabel);
    
    // 创建图表视图
    m_chart = new QChart();
    m_chart->setAnimationOptions(QChart::SeriesAnimations);
    m_chart->legend()->hide();
    
    m_chartView = new QChartView(m_chart);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    
    // 添加组件到主布局
    mainLayout->addWidget(controlPanel);
    mainLayout->addWidget(m_chartView);
    
    // 连接信号
    connect(m_timeSeriesButton, &QPushButton::clicked, this, &QuoteChart::onTimeSeriesButtonClicked);
    connect(m_candlestickButton, &QPushButton::clicked, this, &QuoteChart::onCandlestickButtonClicked);
    connect(m_periodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &QuoteChart::onPeriodChanged);
}

void QuoteChart::createTimeSeriesChart(const StockItem& stock)
{
    clearChart();
    
    // 获取分时数据
    const QVector<TimeSeriesPoint>& timeSeriesData = stock.getTimeSeriesData();
    
    if (timeSeriesData.isEmpty()) {
        m_chart->setTitle(tr("无分时数据"));
        return;
    }
    
    // 创建价格线系列
    m_priceSeries = new QLineSeries();
    m_priceSeries->setName(tr("价格"));
    
    // 创建成交量柱状图系列
    m_volumeSeries = new QBarSeries();
    QBarSet *volumeSet = new QBarSet(tr("成交量"));
    
    // 设置图表标题
    m_chart->setTitle(tr("%1 分时图").arg(stock.getName()));
    
    // 填充数据
    QDateTime minTime = timeSeriesData.first().timestamp;
    QDateTime maxTime = timeSeriesData.last().timestamp;
    
    double minPrice = stock.getLowPrice() * 0.98; // 留一些边距
    double maxPrice = stock.getHighPrice() * 1.02;
    
    long long maxVolume = 0;
    
    for (const TimeSeriesPoint& point : timeSeriesData) {
        // 添加价格点
        m_priceSeries->append(point.timestamp.toMSecsSinceEpoch(), point.price);
        
        // 添加成交量
        *volumeSet << point.volume;
        
        // 更新最大成交量
        if (point.volume > maxVolume) {
            maxVolume = point.volume;
        }
    }
    
    m_volumeSeries->append(volumeSet);
    
    // 创建坐标轴
    m_timeAxis = new QDateTimeAxis();
    m_timeAxis->setFormat("hh:mm");
    m_timeAxis->setRange(minTime, maxTime);
    m_timeAxis->setTickCount(6);
    
    m_priceAxis = new QValueAxis();
    m_priceAxis->setRange(minPrice, maxPrice);
    m_priceAxis->setLabelFormat("%.2f");
    m_priceAxis->setTickCount(5);
    
    m_volumeAxis = new QValueAxis();
    m_volumeAxis->setRange(0, maxVolume * 1.1);
    m_volumeAxis->setLabelFormat("%d");
    m_volumeAxis->setTickCount(3);
    
    // 添加系列到图表
    m_chart->addSeries(m_priceSeries);
    m_chart->addSeries(m_volumeSeries);
    
    // 添加坐标轴到图表
    m_chart->addAxis(m_timeAxis, Qt::AlignBottom);
    m_chart->addAxis(m_priceAxis, Qt::AlignLeft);
    m_chart->addAxis(m_volumeAxis, Qt::AlignRight);
    
    // 关联系列到坐标轴
    m_priceSeries->attachAxis(m_timeAxis);
    m_priceSeries->attachAxis(m_priceAxis);
    
    m_volumeSeries->attachAxis(m_timeAxis);
    m_volumeSeries->attachAxis(m_volumeAxis);
    
    // 设置图表布局
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    
    // 显示图表
    m_chartView->setChart(m_chart);
}

void QuoteChart::createCandlestickChart(const StockItem& stock)
{
    clearChart();
    
    // 获取K线数据
    const QVector<StockTradeData>& kLineData = stock.getKLineData();
    
    if (kLineData.isEmpty()) {
        m_chart->setTitle(tr("无K线数据"));
        return;
    }
    
    // 创建K线图系列
    m_candleSeries = new QCandlestickSeries();
    m_candleSeries->setName(tr("K线"));
    m_candleSeries->setIncreasingColor(QColor(255, 0, 0));
    m_candleSeries->setDecreasingColor(QColor(0, 128, 0));
    
    // 创建成交量柱状图系列
    m_candleVolumeSeries = new QBarSeries();
    QBarSet *volumeSet = new QBarSet(tr("成交量"));
    
    // 获取周期字符串
    QString periodStr;
    switch (m_periodType) {
    case PeriodType::Day:
        periodStr = tr("日K");
        break;
    case PeriodType::Week:
        periodStr = tr("周K");
        break;
    case PeriodType::Month:
        periodStr = tr("月K");
        break;
    case PeriodType::Minutes:
        periodStr = tr("分钟K");
        break;
    case PeriodType::Minutes5:
        periodStr = tr("5分钟K");
        break;
    case PeriodType::Minutes15:
        periodStr = tr("15分钟K");
        break;
    case PeriodType::Minutes30:
        periodStr = tr("30分钟K");
        break;
    case PeriodType::Minutes60:
        periodStr = tr("60分钟K");
        break;
    }
    
    // 设置图表标题
    m_chart->setTitle(tr("%1 %2").arg(stock.getName()).arg(periodStr));
    
    // 填充数据
    QDateTime minTime = kLineData.first().timestamp;
    QDateTime maxTime = kLineData.last().timestamp;
    
    double minPrice = stock.getLowPrice() * 0.98; // 留一些边距
    double maxPrice = stock.getHighPrice() * 1.02;
    
    long long maxVolume = 0;
    
    for (const StockTradeData& data : kLineData) {
        // 创建K线柱
        QCandlestickSet *candleSet = new QCandlestickSet(data.open, data.high, data.low, data.close, data.timestamp.toMSecsSinceEpoch());
        m_candleSeries->append(candleSet);
        
        // 添加成交量
        *volumeSet << data.volume;
        
        // 更新最大成交量
        if (data.volume > maxVolume) {
            maxVolume = data.volume;
        }
        
        // 更新价格范围
        if (data.low < minPrice) {
            minPrice = data.low * 0.98;
        }
        if (data.high > maxPrice) {
            maxPrice = data.high * 1.02;
        }
    }
    
    m_candleVolumeSeries->append(volumeSet);
    
    // 创建坐标轴
    m_timeAxis = new QDateTimeAxis();
    if (m_periodType == PeriodType::Day || m_periodType == PeriodType::Week || m_periodType == PeriodType::Month) {
        m_timeAxis->setFormat("MM-dd");
    } else {
        m_timeAxis->setFormat("hh:mm");
    }
    m_timeAxis->setRange(minTime, maxTime);
    m_timeAxis->setTickCount(6);
    
    m_priceAxis = new QValueAxis();
    m_priceAxis->setRange(minPrice, maxPrice);
    m_priceAxis->setLabelFormat("%.2f");
    m_priceAxis->setTickCount(5);
    
    m_volumeAxis = new QValueAxis();
    m_volumeAxis->setRange(0, maxVolume * 1.1);
    m_volumeAxis->setLabelFormat("%d");
    m_volumeAxis->setTickCount(3);
    
    // 添加系列到图表
    m_chart->addSeries(m_candleSeries);
    m_chart->addSeries(m_candleVolumeSeries);
    
    // 添加坐标轴到图表
    m_chart->addAxis(m_timeAxis, Qt::AlignBottom);
    m_chart->addAxis(m_priceAxis, Qt::AlignLeft);
    m_chart->addAxis(m_volumeAxis, Qt::AlignRight);
    
    // 关联系列到坐标轴
    m_candleSeries->attachAxis(m_timeAxis);
    m_candleSeries->attachAxis(m_priceAxis);
    
    m_candleVolumeSeries->attachAxis(m_timeAxis);
    m_candleVolumeSeries->attachAxis(m_volumeAxis);
    
    // 设置图表布局
    m_chart->layout()->setContentsMargins(0, 0, 0, 0);
    
    // 显示图表
    m_chartView->setChart(m_chart);
}

void QuoteChart::clearChart()
{
    // 清除所有系列和坐标轴
    m_chart->removeAllSeries();
    
    // 删除坐标轴
    if (m_timeAxis) {
        m_chart->removeAxis(m_timeAxis);
        delete m_timeAxis;
        m_timeAxis = nullptr;
    }
    
    if (m_priceAxis) {
        m_chart->removeAxis(m_priceAxis);
        delete m_priceAxis;
        m_priceAxis = nullptr;
    }
    
    if (m_volumeAxis) {
        m_chart->removeAxis(m_volumeAxis);
        delete m_volumeAxis;
        m_volumeAxis = nullptr;
    }
    
    // 释放系列资源
    m_priceSeries = nullptr;  // 系列会由图表删除
    m_volumeSeries = nullptr;
    m_candleSeries = nullptr;
    m_candleVolumeSeries = nullptr;
}

void QuoteChart::showLoadingState(bool isLoading)
{
    // 实现加载状态显示逻辑
} 