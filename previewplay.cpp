#include "previewplay.h"

const QString img_ago (":/lcy/image/ago.png");
const QString img_ago_s(":/lcy/image/ago_s.png");
const QString img_bg (":/lcy/image/bg.png");
const QString img_bg_s (":/lcy/image/bg_s.png");
const QString img_MediaWnd (":/lcy/image/MediaWnd.png");
const QString img_previewPlay (":/lcy/image/previewPlay.png");
const QString img_volume_slider(":/lcy/image/volume_slider.png");
const QString img_previewStop (":/lcy/image/previewStop.png");
const QString img_previewPause (":/lcy/image/previewPause.png");
const QString img_previewMute (":/lcy/image/previewMute.png");
const QString img_previewSound (":/lcy/image/previewSound.png");





PreViewPlay::PreViewPlay(QWidget *parent)
    :QWidget(parent),
      main_gbox(new QGroupBox(this)),
      m_PrePlayProcess(new QProcess)
{

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setSpacing(0);
    QVBoxLayout* insert_layout = new QVBoxLayout(main_gbox);
    insert_layout->setSpacing(0);
    lab_preplay = new QLabel(main_gbox);
//    lab_preplay->setPalette(QPalette(QColor(0,0,0)));
    sdr_process = new QSlider(main_gbox);

    sdr_process->setOrientation(Qt::Horizontal);

    setQSliderStyle(sdr_process,img_volume_slider,img_bg,img_ago);

    setDefaultStyleSheet(lab_preplay,img_MediaWnd);

    insert_layout->addWidget(lab_preplay);
    insert_layout->addWidget(sdr_process);


    QGroupBox* sub_gbox = new QGroupBox();
    QHBoxLayout *hlayout = new QHBoxLayout(sub_gbox);
    hlayout->setSpacing(2);

    btn_play = new QPushButton(sub_gbox);
//    btn_play->setObjectName("btn_play");
    btn_play->setEnabled(false);
    setDefaultStyleSheet(btn_play,img_previewPlay,tr("top"));
    connect(btn_play,SIGNAL(pressed()),SLOT(slot_Play_Clicked()));

    btn_stop = new QPushButton(sub_gbox);
//    btn_stop->setObjectName("btn_stop");
    setDefaultStyleSheet(btn_stop,img_previewStop,tr("bottom"));
    btn_stop->setEnabled(false);
    connect(btn_stop,SIGNAL(pressed()),SLOT(slot_Stop_Clicked()));

    QPushButton *btn_mute = new QPushButton(sub_gbox);
    btn_mute->setObjectName("btn_mute");
    setDefaultStyleSheet(btn_mute,img_previewSound,tr("top"));
    connect(btn_mute,SIGNAL(clicked()),SLOT(slot_Mute_Clicked()));

    QSlider *sdr_volume = new QSlider(sub_gbox);
    sdr_volume->setObjectName("sdr_volume");
    sdr_volume->setRange(0,100);
    sdr_volume->setOrientation(Qt::Horizontal);
    sdr_volume->setValue(50);


    hlayout->addWidget(btn_play);
    hlayout->addWidget(btn_stop);
    hlayout->addWidget(btn_mute);
    hlayout->addWidget(sdr_volume);
    setQSliderStyle(sdr_volume,img_volume_slider,img_bg_s,img_ago_s);





    QHBoxLayout *hsublayout = new QHBoxLayout();
    hsublayout->addWidget(sub_gbox);
    hsublayout->setSpacing(0);
    insert_layout->addLayout(hsublayout);

    main_layout->addWidget(main_gbox);
    setLayout(main_layout);
    connect(sdr_volume,SIGNAL(valueChanged(int)),SLOT(slot_QSlider_Changed(int)));

}

PreViewPlay::~PreViewPlay()
{

}

void PreViewPlay::PrePlayFile(const QString &player,const QString &fname)
{
    QStringList arg;
    arg << "-slave" << "-wid" << QString::number(lab_preplay->winId()) << "-quiet"  << "-vo" << "directx"
        << "-msglevel" << "identify=2"
        << "-loop" << "-1" << fname;
    m_LastPlay.first = player;
    if(!fname.compare(m_LastPlay.second)
            && (m_PrePlayProcess->state() == QProcess::Running))
    {
        return;
    }
    m_LastPlay.second = fname;
    m_PrePlayProcess->kill();
    while(m_PrePlayProcess->waitForFinished());
    m_PrePlayProcess->start(player,arg);
    connect(m_PrePlayProcess,SIGNAL(readyReadStandardOutput()),SLOT(slot_Mplay_recevie()));
    btn_play->setEnabled(true);
    setDefaultStyleSheet(btn_play,img_previewPause,tr("top"));
    btn_stop->setEnabled(true);
    setDefaultStyleSheet(btn_stop,img_previewStop,tr("top"));
}

void PreViewPlay::slot_Mplay_recevie()
{
    m_PrePlayProcess->write("get_percent_pos\n");
    while(m_PrePlayProcess->canReadLine())
    {
        QByteArray ba = m_PrePlayProcess->readLine();
        ba.replace(QByteArray("\r\n"),"");

        if(ba.startsWith("ANS_PERCENT_POSITION"))
        {
            sdr_process->setValue(ba.mid(ba.indexOf("=")+1).toInt());
        }
    }

}

void PreViewPlay::slot_QSlider_Changed(int pos)
{

    pos == this->findChild<QSlider *>("sdr_volume")->minimum()?
        setDefaultStyleSheet(this->findChild<QPushButton*>("btn_mute"),img_previewMute,tr("top")):
          setDefaultStyleSheet(this->findChild<QPushButton*>("btn_mute"),img_previewSound,tr("top"))      ;

}

void PreViewPlay::slot_Play_Clicked()
{

    QString style = btn_play->styleSheet();
    const QString playstyle = "background-image: url("+img_previewPlay+");background-position: top center;margin: -2px -2px -2px -2px;";
    const QString pausestyle = "background-image: url("+img_previewPause+");background-position: top center;margin: -2px -2px -2px -2px;";
    if(!style.compare(playstyle))
     {
         btn_play->setStyleSheet( pausestyle);
         setDefaultStyleSheet(btn_stop,img_previewStop,"top");
         btn_stop->setEnabled(true);
     }
     else
     {
         btn_play->setStyleSheet( playstyle);
         setDefaultStyleSheet(btn_stop,img_previewStop,"bottom");
         btn_stop->setEnabled(false);
         if(!m_PrePlayProcess->state())
         {
           PrePlayFile(m_LastPlay.first,m_LastPlay.second);
           return;
         }
     }


//    if(m_PrePlayProcess)
        m_PrePlayProcess->write("pause\n");

}

void PreViewPlay::slot_Stop_Clicked()
{

//    QString style = btn_stop->styleSheet();
    const QString playstyle = "background-image: url("+img_previewStop+");\
                                 background-position: top center;margin: -2px -2px -2px -2px;";
    const QString stopstyle = "background-image: url("+img_previewStop+");\
                                 background-position: bottom center;margin: -2px -2px -2px -2px;";

    if(btn_stop->isEnabled())
    {
        btn_stop->setStyleSheet(stopstyle);
        setDefaultStyleSheet(btn_play,img_previewPlay,tr("top"));
        if(m_PrePlayProcess)
            m_PrePlayProcess->kill();
        btn_stop->setEnabled(false);
        sdr_process->setValue(0);
    }
    else
    {
        btn_stop->setStyleSheet(playstyle);
    }



}

void PreViewPlay::slot_Mute_Clicked()
{
    QPushButton *w = this->findChild<QPushButton*>("btn_mute");
    QString style = w->styleSheet();
    const QString mute = "background-image: url("+img_previewMute+");\
                                 background-position: top center;margin: -2px -2px -2px -2px;";
    const QString sound = "background-image: url("+img_previewSound+");\
                                 background-position: top center;margin: -2px -2px -2px -2px;";
    QSlider *sld = this->findChild<QSlider*>("sdr_volume");
     if(!style.compare(mute))
     {
         w->setStyleSheet(sound);
         sld->setValue(sld->maximum()/2);
         if(m_PrePlayProcess)
         {
             m_PrePlayProcess->write("mute 0\n");
         }
     }
     else
     {
         w->setStyleSheet(mute);
         sld->setValue(sld->minimum());
         if(m_PrePlayProcess)
         {
             m_PrePlayProcess->write("mute 1\n");
         }
     }
}

void PreViewPlay::switch_Button_state(QWidget *w, const QString &one, const QString &two)
{

    QString style = w->styleSheet();
    const QString onestyle = "background-image: url("+one+");background-position: top center;margin: -2px -2px -2px -2px;";
    const QString twostyle = "background-image: url("+two+");background-position: top center;margin: -2px -2px -2px -2px;";
     w->setStyleSheet(!style.compare(twostyle) ? onestyle : twostyle);
     if(!style.compare(twostyle))
     {
         w->setStyleSheet(onestyle);
     }
     else
     {
     }

}

void PreViewPlay::setDefaultStyleSheet(QWidget *w, const QString &imgname,const QString &position)
{
    w->setStyleSheet("background-image: url("+imgname+"); background-position: "+\
                     position+" center;margin: -2px -2px -2px -2px;");
    w->setFixedSize(27,27);
}

void PreViewPlay::setDefaultStyleSheet(QWidget *w, const QString &imgname)
{
    w->setStyleSheet("background-image: url("+imgname+");");
    QImage img;
    img.load(imgname);
    w->setFixedSize(img.size());
}

void PreViewPlay::setQSliderStyle(QWidget *w,const QString &slider, const QString &add, const QString &sub )
{

    const QString tmp = "QSlider::groove:horizontal { \
                        position: absolute;\
                        height: 10px;\
                        border-radius: 4px;\
                    }\
                    QSlider::handle:horizontal {\
                        height: 13px; width: 13px;\
                        background-image: url(" + slider +");\
                        margin: -1px -1px -1px -1px;\
                    }\
                    QSlider::add-page:horizontal {\
                        border-image: url("+add+");\
                        background-position: top left;\
                    }\
                    QSlider::sub-page:horizontal {\
                        border-image: url("+sub+");\
                        background-position: top left;\
                    }";
    w->setStyleSheet(tmp);
}
