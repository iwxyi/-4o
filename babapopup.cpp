#include <QPainter>
#include <QPainterPath>
#include <QDebug>
#include <QDesktopWidget>
#include <QApplication>
#include <math.h>
#include "babapopup.h"

BaBaPopup::BaBaPopup(QWidget *parent) : QWidget(parent)
{
    setWindowFlag(Qt::FramelessWindowHint, true);
    setWindowFlag(Qt::Tool, true);
    setAttribute(Qt::WA_AlwaysStackOnTop, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
    setFocusPolicy(Qt::NoFocus);

    timer->setInterval(16);
    connect(timer, &QTimer::timeout, this, &BaBaPopup::moveByV);

    QDesktopWidget* desktopWidget =QApplication::desktop();
    QRect deskRect =desktopWidget->availableGeometry();   //获取可用桌面大小
    screenHeight = deskRect.height();
}

void BaBaPopup::setCenter(QPointF gravity, double side)
{
    setFixedSize(int(side), int(side));
    ra = side / 2;
    x = gravity.x();
    y = gravity.y();
    move(int(x - ra), int(y - ra));
}

void BaBaPopup::start(double vx, double vy, double ax, double ay, double a, double aa, double fx)
{
    this->vx = vx;
    this->vy = vy;
    this->ax = ax;
    this->ay = ay;
    this->a = a;
    this->aa = aa;
    this->fx = fx;
    move(int(x - ra), int(y - ra));
    show();
    timer->start();
}

void BaBaPopup::moveByV()
{
    x += vx;
    y += vy;

    vx += ax;
    vy += ay;
    a += aa;

    if (fx > 1e-6)
    {
        if (ax > 0)
        {
            ax -= fx;
            if (ax < 0)
                ax = 0;
        }
        else if (ax < 0)
        {
            ax += fx;
            if (ax > 0)
                ax = 0;
        }
    }

    update();
    move(int(x - ra), int(y - ra));

    if (y > screenHeight)
        this->deleteLater();
}

void BaBaPopup::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(Qt::white, 2));

    // painter.fillRect(rect(), Qt::red);

    double degree = a * 2 * PI / 360;
    painter.drawLine(QPointF(ra + ra * cos(degree), ra - ra * sin(degree)),
                     QPointF(ra - ra * cos(degree), ra + ra * sin(degree)));
}
