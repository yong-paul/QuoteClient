#pragma once

#include "../ui/stocktable.h"
#include "../ui/quotechart.h"
#include "../data/marketdata.h"

#include <QMainWindow>
#include <QTabWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QComboBox>
#include <memory>

namespace Ui {
class MainWindow;
}

/**
 * @brief 主窗口类
 * 
 * 负责整个应用程序的UI布局和交互
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    /**
     * @brief 更新UI显示
     * @param marketData 最新的市场数据
     */
    void updateUI(const MarketData& marketData);

private slots:
    /**
     * @brief 股票表格中选择了新的股票
     * @param code 股票代码
     */
    void onStockSelected(const QString& code);

    /**
     * @brief 切换到分时图
     */
    void showTimeSeriesChart();

    /**
     * @brief 切换到K线图
     */
    void showCandlestickChart();

    /**
     * @brief 刷新数据
     */
    void refreshData();

private:
    /**
     * @brief 初始化UI组件
     */
    void setupUi();

    /**
     * @brief 创建工具栏
     */
    void createToolBar();

    /**
     * @brief 创建状态栏
     */
    void createStatusBar();

private:
    Ui::MainWindow *ui;

    // UI 组件
    QTabWidget* m_tabWidget;
    StockTable* m_stockTable;
    QuoteChart* m_quoteChart;
    
    // 工具栏组件
    QToolBar* m_toolBar;
    QComboBox* m_stockTypeCombo;
    
    // 状态栏组件
    QLabel* m_statusLabel;
    QLabel* m_timeLabel;
    
    // 当前选中的股票代码
    QString m_currentStockCode;
}; 