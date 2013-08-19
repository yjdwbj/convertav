#include "timeslider.h"
#include <QWheelEvent>
#include <QTimer>


TimeSlider::TimeSlider( QWidget * parent )
    : MySlider(parent)
{
    dont_update = false;
    setMinimum(0);
    setMaximum(100);


    setFocusPolicy( Qt::NoFocus );
    setSizePolicy( QSizePolicy::Expanding , QSizePolicy::Fixed );

    connect( this, SIGNAL( sliderPressed() ), this, SLOT( stopUpdate() ) );
    connect( this, SIGNAL( sliderReleased() ), this, SLOT( resumeUpdate() ) );
    connect( this, SIGNAL( sliderReleased() ), this, SLOT( mouseReleased() ) );
    connect( this, SIGNAL( valueChanged(int) ), this, SLOT( valueChanged_slot(int) ) );

    connect( this, SIGNAL(draggingPos(int) ), this, SLOT(checkDragging(int)) );

    last_pos_to_send = -1;
    timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), this, SLOT(sendDelayedPos()) );
    timer->start(200);

}

TimeSlider::~TimeSlider() {
}

void TimeSlider::stopUpdate() {

    dont_update = true;
}

void TimeSlider::resumeUpdate() {
    dont_update = false;
}

void TimeSlider::mouseReleased() {
    emit posChanged( value() );
}

void TimeSlider::valueChanged_slot(int v) {

    // Only to make things clear:
    bool dragging = dont_update;
    if (!dragging) {
        if (v!=position) {
            emit posChanged(v);
        }
    } else {
        emit draggingPos(v);
    }
}


void TimeSlider::setDragDelay(int d) {
       timer->setInterval(d);
}

int TimeSlider::dragDelay() {
    return timer->interval();
}

void TimeSlider::checkDragging(int v) {

    last_pos_to_send = v;
}

void TimeSlider::sendDelayedPos() {
    if (last_pos_to_send != -1) {
        emit delayedDraggingPos(last_pos_to_send);
        last_pos_to_send = -1;
    }
}


void TimeSlider::setPos(int v) {

    if (v!=pos()) {
        if (!dont_update) {
            position = v;
            setValue(v);
        }
    }
}

int TimeSlider::pos() {
    return position;
}

void TimeSlider::wheelEvent( QWheelEvent * e )
{
    e->ignore();
}
