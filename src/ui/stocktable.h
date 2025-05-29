#pragma once

#include "../data/marketdata.h"
#include <QTableView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include <QHeaderView>

/**
 * @brief 股票表格类
 * 
 * 用于显示股票列表和行情数据
 */
class StockTable : public QTableView
{
    Q_OBJECT

public:
    explicit StockTable(QWidget *parent = nullptr);
    ~StockTable();
    
    /**
     * @brief 更新表格数据
     * @param marketData 市场数据
     */
    void updateData(const MarketData& marketData);
    
    /**
     * @brief 设置过滤器，只显示指定市场类型的股票
     * @param type 市场类型
     */
    void setMarketTypeFilter(StockItem::MarketType type);
    
    /**
     * @brief 清除过滤器，显示所有股票
     */
    void clearFilter();

signals:
    /**
     * @brief 选择股票信号
     * @param code 股票代码
     */
    void stockSelected(const QString& code);

protected:
    /**
     * @brief 处理上下文菜单事件
     * @param event 事件对象
     */
    void contextMenuEvent(QContextMenuEvent* event) override;
    
private slots:
    /**
     * @brief 处理表格项选择
     * @param index 选中项索引
     */
    void onSelectionChanged(const QModelIndex& current, const QModelIndex& previous);
    
    /**
     * @brief 处理表头点击
     * @param index 列索引
     */
    void onHeaderClicked(int index);
    
    /**
     * @brief 复制选中的单元格
     */
    void copySelectedCell();
    
    /**
     * @brief 复制选中的行
     */
    void copySelectedRow();

private:
    /**
     * @brief 初始化表格模型
     */
    void setupModel();
    
    /**
     * @brief 设置表格样式
     */
    void setupStyle();
    
    /**
     * @brief 获取股票颜色（涨跌颜色）
     * @param changePercent 涨跌幅
     * @return 颜色
     */
    QColor getStockColor(double changePercent) const;

private:
    QStandardItemModel* m_model;              // 数据模型
    QSortFilterProxyModel* m_proxyModel;      // 排序过滤代理模型
    QMenu* m_contextMenu;                     // 右键菜单
    
    // 当前选中的股票代码
    QString m_selectedStockCode;
    
    // 列索引常量
    enum ColumnIndex {
        ColCode = 0,           // 代码
        ColName = 1,           // 名称
        ColPrice = 2,          // 当前价
        ColChange = 3,         // 涨跌额
        ColChangePercent = 4,  // 涨跌幅
        ColOpen = 5,           // 开盘价
        ColHigh = 6,           // 最高价
        ColLow = 7,            // 最低价
        ColVolume = 8,         // 成交量
        ColAmount = 9          // 成交额
    };
}; 