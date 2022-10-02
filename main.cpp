#include "mainwindow.h"

#include <QApplication>
#include <QSharedMemory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 绑定共享内存
    QSharedMemory shared("share");
    if(shared.attach()) {
        // 绑定成功，则表示共享内存存在
        QMessageBox::warning(NULL, "警告", "程序已经启动!");
        return 0;
    }
    shared.create(1);

    MainWindow w;
    w.show();
    return a.exec();
}
