#ifndef FOUROWIDGET_H
#define FOUROWIDGET_H

#include <QObject>
#include <QWidget>

class FourOWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int dropBarProgress READ getDropBarProgress WRITE setDropBarProgress)
    Q_PROPERTY(int xRotateProgress READ getXRotateProgress WRITE setXRotateProgress)
public:
    explicit FourOWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;

private:
    void setDropBarProgress(const int& x);
    int getDropBarProgress() const;
    void setXRotateProgress(const int& x);
    int getXRotateProgress() const;

private:
    int dropBarProgress = 0;
    int xRotateProgress = 0;
};

#endif // FOUROWIDGET_H
