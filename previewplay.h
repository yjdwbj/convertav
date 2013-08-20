#ifndef PREVIEWPLAY_H
#define PREVIEWPLAY_H
#include <QtWidgets/QLabel>
#include <QtWidgets/QSlider>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QMap>
#include <QProcess>
#include "timeslider.h"

class PreViewPlay: public QWidget
{
    Q_OBJECT
public:
    enum State { Stopped = 0, Playing = 1, Paused = 2 };
    explicit PreViewPlay(QWidget *parent =0);
    ~PreViewPlay();
//
    void PrePlayFile(const QString &player, const QString &fname);

private:
    void setState(State s);
    State state(){return _state;}
    State _state;
//    QGridLayout* main_Layout;
    QGroupBox* main_gbox;
    QLabel *lab_preplay;
    TimeSlider *sdr_process;

    QPushButton* btn_play;

    QPushButton* btn_stop;
    QProcess *m_PrePlayProcess;
    QPair<QString,QString> m_LastPlay;
//    QString m_LastPlay;

    void setDefaultStyleSheet(QWidget *w, const QString &imgname, const QString &position);
    void setDefaultStyleSheet(QWidget *w, const QString &imgname);
    void setQSliderStyle(QWidget *w, const QString &slider, const QString &add, const QString &sub);


    void stop();
    void play();

    void pause_and_frame_step();
    void pause();
    bool isPlaying() const {return m_PrePlayProcess->state() == QProcess::Running;}
    QString seek_cmd(double sec,int mode) const {return QString("seek %1 %2").arg(sec).arg(mode);}
public slots:
    void slot_Stop_Clicked();

private slots:
    void play_or_pause();
    void changePause();

    void slot_Mute_Clicked();
    void slot_Volume_Changed(int );
    void slot_Mplay_recevie();

    void slot_GoToPosition(int);


signals:
    void stateChanged(PreViewPlay::State state);

};

#endif // PREVIEWPLAY_H
