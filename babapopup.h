#ifndef BABAPOPUP_H
#define BABAPOPUP_H

#include <QWidget>
#include <QTimer>

#define PI 3.141592

class BaBaPopup : public QWidget
{
    Q_OBJECT
public:
    explicit BaBaPopup(QWidget *parent = nullptr);

    void setCenter(QPointF gravity, double side);
    void start(double vx, double vy, double ax, double ay, double a, double aa, double fx = 0);

private slots:
    void moveByV();

protected:
    void paintEvent(QPaintEvent *) override;

private:
    double ra = 0;         // 边长的一半
    double x = 0, y = 0;   // 位置（可累计）
    double vx = 0, vy = 0; // 速度
    double ax = 0, ay = 0; // 加速度
    double fx = 0;         // 摩擦力
    double a = 0, aa = 0;  // 旋转角度
    int screenHeight = 0;  // 窗口高度
    QTimer* timer = new QTimer(this);
};

#endif // BABAPOPUP_H
