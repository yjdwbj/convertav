#ifndef MYSLIDER_H
#define MYSLIDER_H

#include <QSlider>

class QTimer;
class MySlider: public QSlider
{
    Q_OBJECT
public:
    explicit MySlider(QWidget *parent = 0);
    ~MySlider();

protected:
    void mousePressEvent(QMouseEvent *ev);
    inline int pick(const QPoint &pt) const;
    int pixelPosToRangeValue(int pos) const;
    void initStyleOption(QStyleOptionSlider *option) const;
};

#endif // MYSLIDER_H
