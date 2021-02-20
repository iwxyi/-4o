#ifndef FOUROWIDGET_H
#define FOUROWIDGET_H

#include <QObject>
#include <QWidget>

#define PI 3.141592

class FourOWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int pullOutProgress READ getPullOutProgress WRITE setPullOutProgress)
    Q_PROPERTY(int droopProgress READ getDroopProgress WRITE setDroopProgress)
    Q_PROPERTY(int downfallProgress READ getDownfallProgress WRITE setDownfallProgress)
    Q_PROPERTY(int xRotateProgress READ getXRotateProgress WRITE setXRotateProgress)
public:
    explicit FourOWidget(QWidget *parent = nullptr);

    void startAnimation();

protected:
    void paintEvent(QPaintEvent *) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void setPullOutProgress(const int& x);
    int getPullOutProgress() const;
    void setDroopProgress(const int& x);
    int getDroopProgress() const;
    void setDownfallProgress(const int& x);
    int getDownfallProgress() const;
    void setXRotateProgress(const int& x);
    int getXRotateProgress() const;

private:
    int pullOutProgress = 0;  // 一阶段：憋红脸拉屎
    int droopProgress = 0;    // 二阶段：耷拉下垂
    int downfallProgress = 0; // 三阶段：坠落
    int xRotateProgress = 0;  // 四阶段：砸中X并开始旋转
};

#endif // FOUROWIDGET_H
