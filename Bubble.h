#ifndef BUBBLE_H
#define BUBBLE_H

#include <QWidget>
#include <QPoint>

namespace Ui {
class Bubble;
}

class Bubble : public QWidget
{
    Q_OBJECT

public:
    explicit Bubble(QWidget *parent = nullptr);
    ~Bubble();

    void initLcd();
    void initBubbleWindow();
    void showMessage();

    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void needClose();

private:
    Ui::Bubble *ui;

    QTimer *m_lcdTimer;

    QTimer *m_pBubbleTimerStart;
    QTimer *m_pBubbleTimerClose;
    QPoint m_point;
    int m_desktopHeight;
    double m_transport;
};

#endif // BUBBLE_H
