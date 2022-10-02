#ifndef BUBBLE_H
#define BUBBLE_H

#include <QWidget>
#include <QPoint>
#include <QSharedPointer>

namespace Ui {
class Bubble;
}

class Bubble : public QWidget
{
    Q_OBJECT

public:
    ~Bubble();

    static QSharedPointer<Bubble>& getInstance() {
        if(m_bubble.isNull()) {
            m_bubble = QSharedPointer<Bubble>(new Bubble());
        }

        return m_bubble;
    }

    void initLcd();
    void initBubbleWindow();
    void showMessage();

    void mouseReleaseEvent(QMouseEvent *event);

signals:
    void needClose();

private:
    explicit Bubble(QWidget *parent = nullptr);
    Bubble(const Bubble&);
    Bubble& operator==(const Bubble&);
    static QSharedPointer<Bubble> m_bubble;

    Ui::Bubble *ui;

    QTimer *m_lcdTimer;

    QTimer *m_pBubbleTimerStart;
    QTimer *m_pBubbleTimerClose;
    QPoint m_point;
    int m_desktopHeight;
    double m_transport;
};

#endif // BUBBLE_H
