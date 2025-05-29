#include "stocktable.h"
#include <QClipboard>
#include <QApplication>
#include <QHeaderView>
#include <QFont>
#include <QBrush>
#include <QMessageBox>

StockTable::StockTable(QWidget *parent)
    : QTableView(parent)
    , m_model(nullptr)
    , m_proxyModel(nullptr)
    , m_contextMenu(nullptr)
{
    setupModel();
    setupStyle();
    
    // 创建右键菜单
    m_contextMenu = new QMenu(this);
    QAction *copyCell = m_contextMenu->addAction(tr("复制单元格"));
    QAction *copyRow = m_contextMenu->addAction(tr("复制行"));
    
    connect(copyCell, &QAction::triggered, this, &StockTable::copySelectedCell);
    connect(copyRow, &QAction::triggered, this, &StockTable::copySelectedRow);
    
    // 连接选择变化信号
    connect(selectionModel(), &QItemSelectionModel::currentChanged,
            this, &StockTable::onSelectionChanged);
    
    // 连接表头点击信号
    connect(horizontalHeader(), &QHeaderView::sectionClicked,
            this, &StockTable::onHeaderClicked);
}

StockTable::~StockTable()
{
    delete m_model;
    delete m_proxyModel;
}

void StockTable::updateData(const MarketData& marketData)
{
    // 先保存当前选中的行
    QString currentCode;
    if (currentIndex().isValid()) {
        currentCode = m_proxyModel->data(m_proxyModel->index(currentIndex().row(), ColCode)).toString();
    }
    
    // 清空模型
    m_model->removeRows(0, m_model->rowCount());
    
    // 填充数据
    const QMap<QString, StockItem>& stocks = marketData.getAllStocks();
    int row = 0;
    
    for (auto it = stocks.constBegin(); it != stocks.constEnd(); ++it) {
        const StockItem& stock = it.value();
        
        // 代码
        QStandardItem *codeItem = new QStandardItem(stock.getCode());
        m_model->setItem(row, ColCode, codeItem);
        
        // 名称
        QStandardItem *nameItem = new QStandardItem(stock.getName());
        m_model->setItem(row, ColName, nameItem);
        
        // 当前价
        QStandardItem *priceItem = new QStandardItem(QString::number(stock.getCurrentPrice(), 'f', 2));
        priceItem->setData(stock.getCurrentPrice(), Qt::UserRole);
        m_model->setItem(row, ColPrice, priceItem);
        
        // 涨跌额
        double change = stock.getChange();
        QStandardItem *changeItem = new QStandardItem(QString::number(change, 'f', 2));
        changeItem->setData(change, Qt::UserRole);
        m_model->setItem(row, ColChange, changeItem);
        
        // 涨跌幅
        double changePercent = stock.getChangePercent();
        QStandardItem *changePercentItem = new QStandardItem(
            QString::number(changePercent, 'f', 2) + "%");
        changePercentItem->setData(changePercent, Qt::UserRole);
        m_model->setItem(row, ColChangePercent, changePercentItem);
        
        // 开盘价
        QStandardItem *openItem = new QStandardItem(QString::number(stock.getOpenPrice(), 'f', 2));
        openItem->setData(stock.getOpenPrice(), Qt::UserRole);
        m_model->setItem(row, ColOpen, openItem);
        
        // 最高价
        QStandardItem *highItem = new QStandardItem(QString::number(stock.getHighPrice(), 'f', 2));
        highItem->setData(stock.getHighPrice(), Qt::UserRole);
        m_model->setItem(row, ColHigh, highItem);
        
        // 最低价
        QStandardItem *lowItem = new QStandardItem(QString::number(stock.getLowPrice(), 'f', 2));
        lowItem->setData(stock.getLowPrice(), Qt::UserRole);
        m_model->setItem(row, ColLow, lowItem);
        
        // 成交量（以万为单位）
        double volumeInWan = stock.getVolume() / 10000.0;
        QStandardItem *volumeItem = new QStandardItem(
            QString::number(volumeInWan, 'f', 0) + tr("万"));
        volumeItem->setData(stock.getVolume(), Qt::UserRole);
        m_model->setItem(row, ColVolume, volumeItem);
        
        // 成交额（以万为单位）
        double amountInWan = stock.getAmount() / 10000.0;
        QStandardItem *amountItem = new QStandardItem(
            QString::number(amountInWan, 'f', 0) + tr("万"));
        amountItem->setData(stock.getAmount(), Qt::UserRole);
        m_model->setItem(row, ColAmount, amountItem);
        
        // 设置颜色
        QColor color = getStockColor(changePercent);
        for (int col = 0; col < m_model->columnCount(); ++col) {
            QStandardItem *item = m_model->item(row, col);
            if (item) {
                item->setForeground(QBrush(color));
            }
        }
        
        row++;
    }
    
    // 恢复之前选中的行
    if (!currentCode.isEmpty()) {
        for (int i = 0; i < m_proxyModel->rowCount(); ++i) {
            QString code = m_proxyModel->data(m_proxyModel->index(i, ColCode)).toString();
            if (code == currentCode) {
                setCurrentIndex(m_proxyModel->index(i, currentIndex().column()));
                break;
            }
        }
    }
}

void StockTable::setMarketTypeFilter(StockItem::MarketType type)
{
    // TODO: 实现市场类型过滤
}

void StockTable::clearFilter()
{
    // TODO: 清除过滤器
}

void StockTable::contextMenuEvent(QContextMenuEvent* event)
{
    if (m_contextMenu) {
        m_contextMenu->exec(event->globalPos());
    }
}

void StockTable::onSelectionChanged(const QModelIndex& current, const QModelIndex& previous)
{
    if (current.isValid()) {
        // 获取当前选中行的股票代码
        QModelIndex codeIndex = m_proxyModel->index(current.row(), ColCode);
        m_selectedStockCode = m_proxyModel->data(codeIndex).toString();
        
        // 发出股票选择信号
        emit stockSelected(m_selectedStockCode);
    }
}

void StockTable::onHeaderClicked(int index)
{
    // 表头点击排序
    sortByColumn(index, horizontalHeader()->sortIndicatorOrder());
}

void StockTable::copySelectedCell()
{
    QModelIndex current = currentIndex();
    if (current.isValid()) {
        QString text = model()->data(current).toString();
        QApplication::clipboard()->setText(text);
    }
}

void StockTable::copySelectedRow()
{
    QModelIndex current = currentIndex();
    if (current.isValid()) {
        int row = current.row();
        QString text;
        
        for (int col = 0; col < model()->columnCount(); ++col) {
            text += model()->data(model()->index(row, col)).toString();
            if (col < model()->columnCount() - 1) {
                text += "\t";
            }
        }
        
        QApplication::clipboard()->setText(text);
    }
}

void StockTable::setupModel()
{
    // 创建数据模型
    m_model = new QStandardItemModel(0, 10, this);
    
    // 设置表头
    m_model->setHeaderData(ColCode, Qt::Horizontal, tr("代码"));
    m_model->setHeaderData(ColName, Qt::Horizontal, tr("名称"));
    m_model->setHeaderData(ColPrice, Qt::Horizontal, tr("当前价"));
    m_model->setHeaderData(ColChange, Qt::Horizontal, tr("涨跌额"));
    m_model->setHeaderData(ColChangePercent, Qt::Horizontal, tr("涨跌幅"));
    m_model->setHeaderData(ColOpen, Qt::Horizontal, tr("开盘价"));
    m_model->setHeaderData(ColHigh, Qt::Horizontal, tr("最高价"));
    m_model->setHeaderData(ColLow, Qt::Horizontal, tr("最低价"));
    m_model->setHeaderData(ColVolume, Qt::Horizontal, tr("成交量"));
    m_model->setHeaderData(ColAmount, Qt::Horizontal, tr("成交额"));
    
    // 创建排序过滤代理模型
    m_proxyModel = new QSortFilterProxyModel(this);
    m_proxyModel->setSourceModel(m_model);
    
    // 设置排序规则
    m_proxyModel->setSortRole(Qt::UserRole);  // 使用UserRole进行排序
    
    // 设置代理模型
    setModel(m_proxyModel);
}

void StockTable::setupStyle()
{
    // 设置表格样式
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setSortingEnabled(true);
    setShowGrid(true);
    
    // 设置表头
    horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSortIndicator(ColChangePercent, Qt::DescendingOrder);
    
    // 设置列宽
    setColumnWidth(ColCode, 80);
    setColumnWidth(ColName, 100);
    setColumnWidth(ColPrice, 80);
    setColumnWidth(ColChange, 80);
    setColumnWidth(ColChangePercent, 80);
    setColumnWidth(ColOpen, 80);
    setColumnWidth(ColHigh, 80);
    setColumnWidth(ColLow, 80);
    setColumnWidth(ColVolume, 100);
    
    // 设置字体
    QFont font = this->font();
    font.setPointSize(10);
    setFont(font);
}

QColor StockTable::getStockColor(double changePercent) const
{
    if (changePercent > 0) {
        return QColor(255, 0, 0);  // 红色表示上涨
    } else if (changePercent < 0) {
        return QColor(0, 128, 0);  // 绿色表示下跌
    } else {
        return QColor(0, 0, 0);    // 黑色表示平盘
    }
} 