#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QTime>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowIcon(QIcon(":/new/icon.jpg"));
    ui->checkBox_common->setChecked(true);
    this->setWindowTitle("提醒小工具");
    ui->lineEdit_timerNum->setFocus();
    m_bubble = Bubble::getInstance();
    loadini();
    initTimer();
    systemTray();
    trayAction();
    startTimer();
}


MainWindow::~MainWindow()
{
    delete ui;
}

// 初始化定时器
void MainWindow::initTimer()
{
    systemTrayIcon = new QSystemTrayIcon(this);
    m_timer = new QTimer(this);
    m_flickerTimer = new QTimer(this);
    m_flickerTimer->setProperty("Property_flag", true);
    m_flickerTimer->setInterval(300);
    ui->pushButton_stop->setEnabled(false);
    // 只能输入数字
    ui->lineEdit_timerNum->setValidator(new QIntValidator(ui->lineEdit_timerNum));
    m_currentTime = 0;

    connect(ui->pushButton_start, &QPushButton::clicked, this, [=](){
        m_isBubbleShow = true;
        ui->lineEdit_timerNum->setEnabled(false);
        ui->pushButton_start->setEnabled(false);
        ui->pushButton_stop->setEnabled(true);

        m_currentTime = ui->lineEdit_timerNum->text().toUInt();
        ui->label_currentTimer->setText(QTime::currentTime().toString() + "\t\t" + QString::number(m_currentTime));

        m_timer->start();
    });
    connect(ui->pushButton_stop, &QPushButton::clicked, this, [=](){
        if(m_timer->isActive()) {
            m_timer->stop();

            ui->lineEdit_timerNum->setEnabled(true);
            ui->pushButton_start->setEnabled(true);
            ui->pushButton_stop->setEnabled(false);
        }
    });
    // 闪烁托盘
    connect(m_flickerTimer, &QTimer::timeout, this, [=](){
        if(m_flickerFlag) {
            m_flickerFlag = false;
            systemTrayIcon->setIcon(m_firIcon);
        }else {
            m_flickerFlag = true;
            systemTrayIcon->setIcon(m_secIcon);
        }
    });
}

// 初始化系统托盘
void MainWindow::systemTray()
{
    // 程序因为没有窗口显示而隐式点击托盘退出才给退出
    QApplication::setQuitOnLastWindowClosed(false);
    this->systemTrayIcon = new QSystemTrayIcon(this);
    this->systemTrayIcon->setIcon(m_secIcon);
    this->systemTrayIcon->setToolTip("定时提醒小工具");
    this->systemTrayIcon->show();
    connect(systemTrayIcon, &QSystemTrayIcon::activated, this, [=](QSystemTrayIcon::ActivationReason reason){
        switch(reason) {
        case QSystemTrayIcon::Trigger:
            // 单击
            resumeTrayIcon();
            break;
        case QSystemTrayIcon::DoubleClick:
            // 双击
            resumeTrayIcon();

            this->showNormal();
            this->activateWindow();
            break;
        default:
            break;
        }
    });
}

// 托盘菜单
void MainWindow::trayAction()
{
    QAction *showMain = new QAction("显示", &menu);
    QAction *exitMain = new QAction("退出", &menu);
    menu.addAction(showMain);
    menu.addAction(exitMain);
    connect(showMain, &QAction::triggered, this, [=](){
        if(this->isHidden())  {
            this->show();
        }
    });
    // 退出
    connect(exitMain, &QAction::triggered, this, [=](){
        setini();
        if(!this->isHidden()) {
            this->hide();
        }
        this->systemTrayIcon->deleteLater();
        this->close();
    });

    this->systemTrayIcon->setContextMenu(&menu);
}

// 定时器开始执行
void MainWindow::startTimer()
{
    m_timer->setInterval(1000);

    connect(m_timer, &QTimer::timeout, this, [=](){
        showToolTips();
        m_currentTime--;
        if(m_currentTime <= 0) {
            m_timer->stop();

            ui->lineEdit_timerNum->setEnabled(true);
            ui->pushButton_start->setEnabled(true);
            ui->pushButton_stop->setEnabled(false);
            checkBoxHandle();
        }

        if(m_currentTime <= 10 && m_isBubbleShow && m_bubble->isHidden()) {
            m_isBubbleShow = false;
            m_bubble->show();
            m_bubble->showMessage();
        }

        ui->label_currentTimer->setText(QTime::currentTime().toString() + "\t\t" + QString::number(m_currentTime));
    });
}

// 托盘提示
void MainWindow::showToolTips()
{
    int timerNum = ui->lineEdit_timerNum->text() == "" ? 0 : ui->lineEdit_timerNum->text().toUInt();
    systemTrayIcon->setToolTip("运行时间：" + QString::number(timerNum - m_currentTime));
}

// 勾选功能执行
void MainWindow::checkBoxHandle()
{
    if(ui->checkBox_flicker->isChecked() && !m_flickerTimer->isActive()) {
//        systemTrayIcon->showMessage("123l", "asd");
        m_flickerTimer->start();
    }
    if(ui->checkBox_common->isChecked()) {
        commonFun();
    }
}

// 普通提示
void MainWindow::commonFun()
{
    QMessageBox *msgBox = new QMessageBox(QMessageBox::Information, tr("提示"), tr("休息一会~"), QMessageBox::Yes, NULL, Qt::WindowStaysOnTopHint);
    // 关闭时自动删除
    msgBox->setAttribute( Qt::WA_DeleteOnClose );
    // 非模态
    msgBox->setModal(false);
    msgBox->open();
}

// 托盘闪烁恢复
void MainWindow::resumeTrayIcon()
{
    if(m_flickerTimer->isActive()) {
        m_flickerTimer->stop();
        systemTrayIcon->setIcon(m_secIcon);
    }
}

// 加载配置文件
void MainWindow::loadini()
{
    QString location= QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    location += "/clock_timer.ini";
    // 判断文件是否存在
    QFileInfo fileInfo(location);
    if(fileInfo.isFile()) {
        // 读取配置文件
        QSettings settings(location, QSettings::IniFormat);
        ui->lineEdit_timerNum->setText(settings.value("timer").toString());
        ui->checkBox_common->setChecked(settings.value("common").toBool());
        ui->checkBox_flicker->setChecked(settings.value("flicker").toBool());
    }

}

void MainWindow::setini()
{
    QString location= QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    location += "/clock_timer.ini";
    QSettings settings(location, QSettings::IniFormat);
    settings.setValue("timer", ui->lineEdit_timerNum->text());
    bool isCommon = ui->checkBox_common->isChecked();
    bool isFlicker = ui->checkBox_flicker->isChecked();
    settings.setValue("common", isCommon);
    settings.setValue("flicker", isFlicker);
    settings.sync();

}

// 最小化到托盘
void MainWindow::changeEvent(QEvent *e)
{
    if((e->type() == QEvent::WindowStateChange) && this->isMinimized()) {
        this->hide();
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(!this->isHidden()) {
        this->hide();
        e->ignore();
    }else {
        qApp->quit();
    }
}


