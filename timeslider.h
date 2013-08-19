#ifndef TIMESLIDER_H
#define TIMESLIDER_H
#include "myslider.h"


class TimeSlider : public MySlider
{
    Q_OBJECT

public:
    TimeSlider( QWidget * parent );
    ~TimeSlider();

public slots:
    virtual void setPos(int); // Don't use setValue!
    virtual int pos();

    void setDragDelay(int);
    int dragDelay();


signals:
    void posChanged(int);
    void draggingPos(int);

    //! Emitted with a few ms of delay
    void delayedDraggingPos(int);


protected slots:
    void stopUpdate();
    void resumeUpdate();
    void mouseReleased();
    void valueChanged_slot(int);

    void checkDragging(int);
    void sendDelayedPos();


    virtual void wheelEvent( QWheelEvent * e );

private:
    bool dont_update;
    int position;


    int last_pos_to_send;
    QTimer * timer;
};
#endif // TIMESLIDER_H
