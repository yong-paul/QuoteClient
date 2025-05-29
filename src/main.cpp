#include "app/application.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    QApplication::setApplicationName("QuoteClient");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("QuoteOrg");
    
    // 初始化应用程序实例
    Application quoteApp;
    quoteApp.initialize();
    
    return app.exec();
} 