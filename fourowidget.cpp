#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QKeyEvent>
#include <QDebug>
#include <math.h>
#include "fourowidget.h"
#include "babapopup.h"

FourOWidget::FourOWidget(QWidget *parent) : QWidget(parent)
{
    pullOutProgress = droopProgress = downfallProgress = xRotateProgress = 0;

    this->setFocusPolicy(Qt::StrongFocus);
    // this->setFocus(); // 构造函数这里focus会有几率导致崩溃，以及未初始化时update
}

void FourOWidget::startAnimation()
{
    pullOutProgress = droopProgress = downfallProgress = xRotateProgress = 0;

    if (group)
    {
        group->stop();
        group->deleteLater();
    }

    group = new QSequentialAnimationGroup(this);
    connect(group, &QSequentialAnimationGroup::finished, this, [=]{
        group->deleteLater();
        group = nullptr;
    });

    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "pullOutProgress");
        ani->setStartValue(0);
        ani->setEndValue(100);
        ani->setDuration(1000);
        ani->setEasingCurve(QEasingCurve::OutCubic);
        group->addAnimation(ani);
    }

    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "droopProgress");
        ani->setStartValue(0);
        ani->setEndValue(100);
        ani->setDuration(600);
        ani->setEasingCurve(QEasingCurve::OutBounce);
        group->addAnimation(ani);
    }

    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "downfallProgress");
        ani->setStartValue(0);
        ani->setEndValue(100);
        ani->setDuration(500);
        ani->setEasingCurve(QEasingCurve::InQuint);
        group->addAnimation(ani);
    }

    {
        QPropertyAnimation* ani = new QPropertyAnimation(this, "xRotateProgress");
        ani->setStartValue(0);
        ani->setEndValue(X_ROTATE_ANGLE);
        ani->setDuration(5000);
        ani->setEasingCurve(QEasingCurve::OutQuart);
        group->addAnimation(ani);

        connect(ani, &QPropertyAnimation::stateChanged, this, [=](QAbstractAnimation::State newState, QAbstractAnimation::State) {
            if (newState == QAbstractAnimation::Running)
            {
                popBaBa();
            }
        });
    }

    group->start();
}

/**
 * 把粑粑弹出到屏幕外面去
 */
void FourOWidget::popBaBa()
{
    BaBaPopup* bbp = new BaBaPopup(this);
    bbp->setCenter(popPoint + mapToGlobal(QPoint()), babaLen);
    bbp->start(-5, -0.5, 0.5, 0.3, 90, 3, 0.1);
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
    const QColor suppressColor = QColor(205, 92, 92);

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

    // X 属性
    const double xPadding = padding;
    QRectF xRect(xPadding, xPadding + statusBarHeight, appBarHeight - xPadding*2, appBarHeight - xPadding*2);

    // 画掉落的 -
    const double barLen = widthOfN * 0.8;
    const double delta = widthOfN * 0.25; // 中间绕着空白的长度
    const double barRight = paddingLeft + barLen + widthOfN * 0.1;
    const double barTop = textBottom - fontHeight / 4; // 纵坐标，根据字体4的位置慢慢调
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
        const double rightX = barRight + delta * droopProgress / 100;
        const double leftX = rightX - cos(degree) * barLen;
        const double leftY = barTop + sin(degree) * barLen;

        painter.drawLine(QPointF(leftX, leftY), QPointF(rightX, barTop + delta * droopProgress / 100));
    }
    else if (downfallProgress && !xRotateProgress) // 阶段三：坠落
    {
        const double top = barTop + delta;
        const double bottom = xRect.top();
        const double x = barRight + delta;
        const double y = top + (bottom - top - barLen / 2) * downfallProgress / 100;

        painter.drawLine(QPointF(x, y), QPointF(x, y + barLen));

        this->babaLen = barLen;
        this->popPoint = xRect.topLeft() + QPointF(widthOfN * 0.1, 0);
    }

    // 画旋转的 X
    if (!xRotateProgress) // 正常状态下的X
    {
        painter.drawLine(xRect.topLeft(), xRect.bottomRight());
        painter.drawLine(xRect.topRight(), xRect.bottomLeft());
    }
    else
    {
        auto toDegree = [](double angle) {
            return angle * 2 * PI / 360;
        };
        const int turns = 3; // 绕的圈数
        const QPointF center = xRect.center();
        const double radius = sqrt(xRect.width() * xRect.width() / 2);
        const double angle = 360 * turns * xRotateProgress / X_ROTATE_ANGLE;

        auto getPos = [=](double rotate) {
            return QPointF(
                        center.x() + sin(toDegree(rotate + angle)) * radius,
                        center.y() + cos(toDegree(rotate + angle)) * radius
                        );
        };

        painter.drawLine(getPos(135), getPos(-45));
        painter.drawLine(getPos(45), getPos(-135));
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
