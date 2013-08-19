#include "myslider.h"

#include <QApplication>
#include <QMouseEvent>

#include <QStyle>
#include <QStyleOption>

MySlider::MySlider(QWidget *parent)
    :QSlider(parent)
{
    setOrientation(Qt::Horizontal);
}

MySlider::~MySlider()
{

}

inline int MySlider::pick(const QPoint &pt) const
{
    return orientation() == Qt::Horizontal ? pt.x() : pt.y();
}

int MySlider::pixelPosToRangeValue(int pos) const
{
    QStyleOptionSlider opt;
    initStyleOption(&opt);
    QRect gr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, this);
    QRect sr = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
    int sliderMin, sliderMax, sliderLength;

    if (orientation() == Qt::Horizontal) {
        sliderLength = sr.width();
        sliderMin = gr.x();
        sliderMax = gr.right() - sliderLength + 1;
    } else {
        sliderLength = sr.height();
        sliderMin = gr.y();
        sliderMax = gr.bottom() - sliderLength + 1;
    }
    return QStyle::sliderValueFromPosition(minimum(), maximum(), pos - sliderMin,
                                           sliderMax - sliderMin, opt.upsideDown);
}


void MySlider::mousePressEvent( QMouseEvent * e ) {
    if (e->button() == Qt::LeftButton) {
        QStyleOptionSlider opt;
        initStyleOption(&opt);
        const QRect sliderRect = style()->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, this);
        const QPoint center = sliderRect.center() - sliderRect.topLeft();
        // to take half of the slider off for the setSliderPosition call we use the center - topLeft

        if (!sliderRect.contains(e->pos())) {
            e->accept();

            setSliderPosition(pixelPosToRangeValue(pick(e->pos() - center)));
            triggerAction(SliderMove);
            setRepeatAction(SliderNoAction);
        } else {
            QSlider::mousePressEvent(e);
        }
    } else {
        QSlider::mousePressEvent(e);
    }
}

void MySlider::initStyleOption(QStyleOptionSlider *option) const
{
    if (!option)
        return;

    option->initFrom(this);
    option->subControls = QStyle::SC_None;
    option->activeSubControls = QStyle::SC_None;
    option->orientation = orientation();
    option->maximum = maximum();
    option->minimum = minimum();
    option->tickPosition = (QSlider::TickPosition) tickPosition();
    option->tickInterval = tickInterval();
    option->upsideDown = (orientation() == Qt::Horizontal) ?
                     (invertedAppearance() != (option->direction == Qt::RightToLeft))
                     : (!invertedAppearance());
    option->direction = Qt::LeftToRight; // we use the upsideDown option instead
    option->sliderPosition = sliderPosition();
    option->sliderValue = value();
    option->singleStep = singleStep();
    option->pageStep = pageStep();
    if (orientation() == Qt::Horizontal)
        option->state |= QStyle::State_Horizontal;
}
