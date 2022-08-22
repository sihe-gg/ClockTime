#include "Bubble.h"
#include "ui_bubble.h"
#include <QTimer>
#include <QTime>
#include <QScreen>
#include <QRect>
#include <QLCDNumber>

Bubble::Bubble(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Bubble)
{
    ui->setupUi(this);
    initLcd();
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    m_desktopHeight = QApplication::screens().at(0)->size().rheight();

    m_transport = 1.0;
    m_lcdTimer = new QTimer(this);
    m_lcdTimer->setInterval(1000);
    m_pBubbleTimerStart = new QTimer(this);
    m_pBubbleTimerStay = new QTimer(this);
    m_pBubbleTimerClose = new QTimer(this);

    showMessage("时间到了，休息一下！");

    connect(m_pBubbleTimerStart, &QTimer::timeout, this, [=](){
        // 5s 后开始上升
        m_desktopHeight--; // 桌面高度减一，窗口坐标就会增高
        this->move(m_point.x(), m_desktopHeight);
        if(m_desktopHeight <= m_point.y()) {
            m_pBubbleTimerStart->stop();
            m_pBubbleTimerStay->start(15000);// 设置停留时间
        }
    });
    connect(m_pBubbleTimerStay, &QTimer::timeout, this, [=](){
        m_pBubbleTimerStay->stop();
        m_pBubbleTimerClose->start(100);
    });
    connect(m_pBubbleTimerClose, &QTimer::timeout, this, [=](){
        m_transport -= 0.1;
        if(m_transport <= 0.0) {
            m_pBubbleTimerClose->stop();
            this->close();
        }else {
            this->setWindowOpacity(m_transport);
        }
    });
    connect(m_lcdTimer, &QTimer::timeout, this, [=](){
        QString curTime = QTime::currentTime().toString();
        ui->lcdNumber->display(curTime);
    });

    m_lcdTimer->start();
}

Bubble::~Bubble()
{
    delete ui;
}

void Bubble::initLcd()
{
    ui->lcdNumber->setDigitCount(8);
    ui->lcdNumber->setSegmentStyle(QLCDNumber::Flat);
    ui->lcdNumber->setMode(QLCDNumber::Dec);
    ui->lcdNumber->setStyleSheet("font: 48px;");
}

void Bubble::showMessage(const char *str)
{
    ui->label_msg->setStyleSheet("background-color: rgb(255,210,200); font: 48px; color: blue;");
    ui->label_msg->setText(str);
    ui->label_msg->setAlignment(Qt::AlignCenter);
    QRect rect = QApplication::screens().at(0)->availableGeometry();// 获取屏幕大小
    m_point.setX(rect.width() - this->width());
    m_point.setY(rect.height() - this->height());
    this->move(m_point.x(), m_point.y());

    QString curTime = QTime::currentTime().toString();
    ui->lcdNumber->display(curTime);

    m_pBubbleTimerStart->start(5);
}

void Bubble::mouseReleaseEvent(QMouseEvent *event)
{
    this->close();
}
