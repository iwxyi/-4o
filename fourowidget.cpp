#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QKeyEvent>
#include <QDebug>
#include <math.h>
#include "fourowidget.h"

FourOWidget::FourOWidget(QWidget *parent) : QWidget(parent)
{
    this->setFocusPolicy(Qt::StrongFocus);
    this->setFocus();
}

void FourOWidget::startAnimation()
{
    pullOutProgress = droopProgress = downfallProgress = xRotateProgress = 0;

    QSequentialAnimationGroup* group = new QSequentialAnimationGroup(this);
    connect(group, SIGNAL(finished()), group, SLOT(deleteLater()));

    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "pullOutProgress");
        ani->setStartValue(0);
        ani->setEndValue(100);
        ani->setDuration(500);
        ani->setEasingCurve(QEasingCurve::OutCubic);
        group->addAnimation(ani);
    }

    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "droopProgress");
        ani->setStartValue(0);
        ani->setEndValue(100);
        ani->setDuration(500);
        ani->setEasingCurve(QEasingCurve::OutBounce);
        group->addAnimation(ani);
    }

    group->start();
}

void FourOWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // 字体属性
    QFont font;
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);

    const QFontMetricsF fm(font);
    const double fontHeight = fm.height();
    const double lineSpacing = fm.lineSpacing();

    const int statusBarHeight = int(fontHeight + (lineSpacing-fontHeight)*3);
    const int appBarHeight = int(statusBarHeight * 2);

    // 画MD背景
    const QColor statusBarColor = QColor(0, 77, 188);
    const QColor appBarColor = QColor(0, 166, 254);
    painter.fillRect(0, 0, width(), statusBarHeight, statusBarColor);
    painter.fillRect(0, statusBarHeight, width(), appBarHeight, appBarColor);
    painter.fillRect(0, statusBarHeight + appBarHeight, width(), height(), Qt::white);

    // 文字属性
    const double paddingLeft = (lineSpacing - fontHeight) * 3;
    const double widthOfN = fm.horizontalAdvance("-");
    const double widthOf4 = fm.horizontalAdvance("4");
    const double textBottom = lineSpacing;
    const QColor textColor = Qt::white;
    const QColor suppressColor = Qt::red;

    // 画固定的 4°
    painter.setPen(QPen(textColor, 2));
    double padding = paddingLeft;
    painter.drawText(QPointF(padding += widthOfN, textBottom), "4");
    painter.drawText(QPointF(padding + widthOf4 * 2 / 3, textBottom - 2), "°");
    if (pullOutProgress && !droopProgress) // 正在憋屎，脸涨红
    {
        painter.save();
        QColor c = suppressColor;
        c.setAlpha(255 * pullOutProgress / 100);
        painter.setPen(c);
        painter.drawText(QPointF(padding + widthOf4 * 2 / 3, textBottom - 2), "°");
        painter.restore();
    }


    // 画旋转的 X
    const double xPadding = padding;
    QRectF xRect(xPadding, xPadding + statusBarHeight, appBarHeight - xPadding*2, appBarHeight - xPadding*2);
    painter.drawLine(xRect.topLeft(), xRect.bottomRight());
    painter.drawLine(xRect.topRight(), xRect.bottomLeft());


    // 画掉落的 -
    const double barLen = widthOfN * 0.8;
    const double barRight = paddingLeft + barLen + widthOfN * 0.1;
    const double barTop = textBottom - fontHeight / 3.6; // 纵坐标，根据字体4的位置慢慢调
    const double barLeft = barRight - barLen * pullOutProgress / 100.0;
    if (!pullOutProgress) // 静态
    {}
    else if (pullOutProgress && !droopProgress) // 阶段一：拉出来
    {
        painter.drawLine(QPointF(barLeft, barTop), QPointF(barRight, barTop));
    }
    else if (droopProgress && !downfallProgress) // 阶段二：耷拉下垂
    {
        const double angle = 90 * droopProgress / 100;
        const double degree = angle * 2 * PI / 360;
        const double leftX = barRight - cos(degree) * barLen;
        const double leftY = barTop + sin(degree) * barLen;
        painter.drawLine(QPointF(leftX, leftY), QPointF(barRight, barTop));
    }
    else if (downfallProgress && !xRotateProgress) // 阶段三：坠落
    {

    }
    else
    {

    }
}

void FourOWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Space)
    {
        startAnimation();
    }
    return QWidget::keyPressEvent(event);
}

void FourOWidget::setPullOutProgress(const int &x)
{
    this->pullOutProgress = x;
    update();
}

int FourOWidget::getPullOutProgress() const
{
    return pullOutProgress;
}

void FourOWidget::setDroopProgress(const int &x)
{
    this->droopProgress = x;
    update();
}

int FourOWidget::getDroopProgress() const
{
    return droopProgress;
}

void FourOWidget::setDownfallProgress(const int &x)
{
    this->downfallProgress = x;
    update();
}

int FourOWidget::getDownfallProgress() const
{
    return downfallProgress;
}

void FourOWidget::setXRotateProgress(const int &x)
{
    this->xRotateProgress = x;
    update();
}

int FourOWidget::getXRotateProgress() const
{
    return xRotateProgress;
}
