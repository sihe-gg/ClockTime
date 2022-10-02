#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bubble.h"
#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QPushButton>
#include <QIntValidator>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initTimer();

    // 托盘
    void systemTray();
    void trayAction();
    void startTimer();
    void showToolTips();
    void checkBoxHandle();

    // 提醒模式
    void commonFun();
    void resumeTrayIcon();

    // 读写配置文件
    void loadini();
    void setini();

    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *e);

signals:
    void triggerClose();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *systemTrayIcon;

    QSharedPointer<Bubble> m_bubble;

    QMenu menu;
    QTimer *m_timer;
    QTimer *m_flickerTimer;

    int m_currentTime;

    bool m_isBubbleShow = false;     //判断是否最后 10s 弹出提醒框
    bool m_flickerFlag = false;      // 托盘闪烁换图标判断

    const QIcon m_firIcon = QIcon(":/new/dtb25.png");
    const QIcon m_secIcon = QIcon(":/new/icon.jpg");
};
#endif // MAINWINDOW_H
