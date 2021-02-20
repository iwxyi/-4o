#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include "fourowidget.h"

FourOWidget::FourOWidget(QWidget *parent) : QWidget(parent)
{

}

void FourOWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

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
    double paddingLeft = (lineSpacing - fontHeight) * 3;
    const double widthOfN = fm.horizontalAdvance("-");
    const double widthOf4 = fm.horizontalAdvance("4");
    const double textBottom = lineSpacing;
    const QColor textColor = Qt::white;
    const QColor suppressColor = Qt::red;

    // 画固定的 4°
    painter.setPen(QPen(textColor, 2));
    painter.drawText(QPointF(paddingLeft, textBottom), "-");
    painter.drawText(QPointF(paddingLeft += widthOfN, textBottom), "4");
    painter.drawText(QPointF(paddingLeft + widthOf4 * 2 / 3, textBottom - 2), "°");


    // 画旋转的 X
    const double xPadding = paddingLeft;
    QRectF xRect(xPadding, xPadding + statusBarHeight, appBarHeight - xPadding*2, appBarHeight - xPadding*2);
    painter.drawLine(xRect.topLeft(), xRect.bottomRight());
    painter.drawLine(xRect.topRight(), xRect.bottomLeft());


    // 画掉落的 -

}

void FourOWidget::setDropBarProgress(const int &x)
{
    this->dropBarProgress = x;
}

int FourOWidget::getDropBarProgress() const
{
    return dropBarProgress;
}

void FourOWidget::setXRotateProgress(const int &x)
{
    this->xRotateProgress = x;
}

int FourOWidget::getXRotateProgress() const
{
    return xRotateProgress;
}
