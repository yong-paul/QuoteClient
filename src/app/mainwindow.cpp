#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QAction>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include <QSplitter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)
    , m_tabWidget(nullptr)
    , m_stockTable(nullptr)
    , m_quoteChart(nullptr)
    , m_toolBar(nullptr)
    , m_stockTypeCombo(nullptr)
    , m_statusLabel(nullptr)
    , m_timeLabel(nullptr)
{
    setWindowTitle(tr("证券行情客户端"));
    resize(1024, 768);
    
    setupUi();
    createToolBar();
    createStatusBar();
    
    // 更新时间
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, [this]() {
        m_timeLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    });
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUi()
{
    // 创建中央小部件
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // 创建垂直布局
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // 创建分割器，使面板可调整大小
    QSplitter *splitter = new QSplitter(Qt::Vertical);
    mainLayout->addWidget(splitter);
    
    // 创建股票表格
    m_stockTable = new StockTable();
    splitter->addWidget(m_stockTable);
    
    // 创建行情图表区域
    QWidget *chartWidget = new QWidget();
    QVBoxLayout *chartLayout = new QVBoxLayout(chartWidget);
    
    // 创建行情图表
    m_quoteChart = new QuoteChart();
    chartLayout->addWidget(m_quoteChart);
    
    splitter->addWidget(chartWidget);
    
    // 设置初始分割比例
    splitter->setSizes(QList<int>() << 300 << 400);
    
    // 连接股票选择信号
    connect(m_stockTable, &StockTable::stockSelected, this, &MainWindow::onStockSelected);
}

void MainWindow::createToolBar()
{
    m_toolBar = addToolBar(tr("工具栏"));
    m_toolBar->setMovable(false);
    
    // 添加刷新按钮
    QAction *refreshAction = m_toolBar->addAction(tr("刷新"));
    connect(refreshAction, &QAction::triggered, this, &MainWindow::refreshData);
    
    m_toolBar->addSeparator();
    
    // 添加股票类型选择下拉框
    QLabel *typeLabel = new QLabel(tr("股票类型: "));
    m_toolBar->addWidget(typeLabel);
    
    m_stockTypeCombo = new QComboBox();
    m_stockTypeCombo->addItem(tr("全部"));
    m_stockTypeCombo->addItem(tr("上证A股"));
    m_stockTypeCombo->addItem(tr("深证A股"));
    m_stockTypeCombo->addItem(tr("创业板"));
    m_stockTypeCombo->addItem(tr("科创板"));
    m_toolBar->addWidget(m_stockTypeCombo);
    
    m_toolBar->addSeparator();
    
    // 添加图表类型按钮
    QAction *timeSeriesAction = m_toolBar->addAction(tr("分时图"));
    connect(timeSeriesAction, &QAction::triggered, this, &MainWindow::showTimeSeriesChart);
    
    QAction *candlestickAction = m_toolBar->addAction(tr("K线图"));
    connect(candlestickAction, &QAction::triggered, this, &MainWindow::showCandlestickChart);
}

void MainWindow::createStatusBar()
{
    m_statusLabel = new QLabel(tr("就绪"));
    statusBar()->addWidget(m_statusLabel);
    
    m_timeLabel = new QLabel();
    statusBar()->addPermanentWidget(m_timeLabel);
}

void MainWindow::updateUI(const MarketData& marketData)
{
    // 更新股票表格
    m_stockTable->updateData(marketData);
    
    // 如果有选中的股票，则更新图表
    if (!m_currentStockCode.isEmpty()) {
        const StockItem *stock = marketData.getStock(m_currentStockCode);
        if (stock) {
            m_quoteChart->updateChart(*stock);
        }
    }
    
    // 更新状态栏
    m_statusLabel->setText(tr("数据已更新 - %1")
                          .arg(QDateTime::currentDateTime().toString("hh:mm:ss")));
}

void MainWindow::onStockSelected(const QString& code)
{
    m_currentStockCode = code;
    // 当选择了新股票时，通知其他组件
    emit m_quoteChart->stockChanged(code);
}

void MainWindow::showTimeSeriesChart()
{
    m_quoteChart->setChartType(QuoteChart::ChartType::TimeSeries);
}

void MainWindow::showCandlestickChart()
{
    m_quoteChart->setChartType(QuoteChart::ChartType::Candlestick);
}

void MainWindow::refreshData()
{
    // 仅发出请求刷新的信号，具体刷新逻辑由数据管理层处理
    m_statusLabel->setText(tr("正在刷新数据..."));
} 