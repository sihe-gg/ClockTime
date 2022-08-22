#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Bubble.h"
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
    void bubbleFun();
    void flickerFun();

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

    QMenu menu;
    QTimer *m_timer;
    QTimer *m_flickerTimer;

    int m_currentTime;
};
#endif // MAINWINDOW_H
