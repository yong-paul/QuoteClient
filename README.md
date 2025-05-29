# QuoteClient - 证券行情客户端

一个基于Qt和C++开发的证券行情客户端，提供基本的行情展示功能。

## 功能特点

- 股票行情列表显示
- 分时图和K线图展示
- 支持多种K线周期（日K、周K、月K、分钟K等）
- 实时数据更新
- 自定义筛选功能

## 技术架构

- **UI层**：使用Qt实现界面展示
- **数据层**：使用C++实现数据处理
- **网络层**：负责数据获取和通信

### 模块划分

- **App**：应用程序主模块，负责协调各组件
- **UI**：用户界面模块，包含股票表格和行情图表
- **Data**：数据管理模块，处理市场数据和股票信息
- **Network**：网络模块，负责数据获取和通信

## 开发环境

- Qt 6.x
- C++17
- Visual Studio 2022
- CMake 3.16+

## 构建方法

### 使用CMake构建

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### 使用Visual Studio 2022构建

1. 打开VS2022，选择"打开CMake"
2. 选择项目根目录下的CMakeLists.txt
3. 选择构建目标并构建

## 使用说明

1. 启动应用后，将自动加载模拟数据
2. 在股票列表中选择股票，查看详细行情
3. 可切换分时图和K线图
4. 可选择不同的K线周期
5. 使用刷新按钮更新数据

## 项目结构

```
src/
├── app/              # 应用程序主模块
│   ├── application.h/cpp
│   └── mainwindow.h/cpp
├── data/             # 数据管理模块
│   ├── marketdata.h/cpp
│   ├── stockitem.h/cpp
│   └── datamanager.h/cpp
├── network/          # 网络模块
│   └── dataprovider.h/cpp
├── ui/               # 用户界面模块
│   ├── stocktable.h/cpp
│   └── quotechart.h/cpp
└── resources/        # 资源文件
    └── resources.qrc
```

## 许可证

MIT许可证 