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

class PreViewPlay: public QWidget
{
    Q_OBJECT
public:
    explicit PreViewPlay(QWidget *parent =0);
    ~PreViewPlay();
//
    void PrePlayFile(const QString &player, const QString &fname);
private:
//    QGridLayout* main_Layout;
    QGroupBox* main_gbox;
    QLabel *lab_preplay;
    QSlider *sdr_process;

    QPushButton* btn_play;
    QPushButton* btn_stop;

    QProcess *m_PrePlayProcess;
    void setDefaultStyleSheet(QWidget *w, const QString &imgname, const QString &position);
    void setDefaultStyleSheet(QWidget *w, const QString &imgname);
    void setQSliderStyle(QWidget *w, const QString &slider, const QString &add, const QString &sub);
    void switch_Button_state(QWidget *w,const QString &one,const QString &two);
private slots:
    void slot_Play_Clicked();
    void slot_Stop_Clicked();
    void slot_Mute_Clicked();
    void slot_QSlider_Changed(int );
    void slot_Mplay_recevie();

};

#endif // PREVIEWPLAY_H
