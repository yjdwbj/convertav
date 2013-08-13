#include "itemoflistfiles.h"
#include <QFrame>
ItemView::ItemView(itemstruct &item, const ToolBoxSettings *tbs, QWidget *parent)
    :QTabWidget(parent),
      main_layout(new QGridLayout),
      btn_convert(new QPushButton),
      btn_delself(new QPushButton),
//      lab_audio(new QLabel),
//      lab_videoinfo(new QLabel),
      cbox_selelct(new QCheckBox),
      lab_view(new QLabel("test")),
      m_mencoder(qApp->applicationDirPath() + ("/mencoder.exe"))
//      lab_time(new QLabel)
{
     m_item = item;
     m_ToolBoxSettings = tbs;
    this->setStyleSheet("QProgressBar {"
                        "background-image: url(:/lcy/image/progress_bk.png);}"
                        "QProgressBar::chunk {"
                        "background-image: url(:/lcy/image/progress_fg.png);}");
    QHBoxLayout *oneline = new QHBoxLayout;
    oneline->addWidget(cbox_selelct);

    oneline->addStretch(10);
    btn_convert->setToolTip(tr("开始转换"));
    btn_convert->setFixedSize(25,19);
    btn_convert->setIconSize(btn_convert->size());
    btn_convert->setIcon(QPixmap(":/lcy/image/converter.png").copy(0,37,25,19));
    btn_convert->setObjectName("btn_convert");
    connect(btn_convert,SIGNAL(pressed()),SLOT(slot_MouseOnConvert()));
    connect(this,SIGNAL(parentConvertSignal()),SLOT(slot_MouseOnConvert()));
    oneline->addWidget(btn_convert);
    btn_delself->setFixedSize(39,19);
    btn_delself->setIconSize(btn_delself->size());
    btn_delself->setIcon(QPixmap(":/lcy/image/no.png").copy(0,0,39,19));
    btn_delself->setToolTip("删除该项");
    connect(btn_delself,SIGNAL(clicked()),SLOT(slot_destoryMySelf()));

    oneline->addWidget(btn_delself);
    lab_view->setFrameShape(QFrame::Panel);
    QImage img(m_item.image);

    lab_view->setFixedSize(68,54);
    lab_view->setPixmap(QPixmap::fromImage(img).scaled(68,54));

    main_layout->addWidget(lab_view,0,0,2,1);

    cbox_selelct->setCheckState(Qt::Checked);


//    main_layout->addWidget(btn_convert);

    main_layout->addLayout(oneline,0,1);
    main_layout->addLayout(CreateItemInfoLayout(),1,1);
    main_layout->setSpacing(5);
    QWidget *w=new QWidget;
    w->setLayout(main_layout);
    addTab(w,item.filename);
    this->setToolTip(item.filename);
    fullpath = item.fullpath;


}

QLayout* ItemView::CreateItemInfoLayout()
{
    QHBoxLayout *twoline = new QHBoxLayout;
    QLabel* ico_time = new QLabel;
    setDefaultStyleSheet(ico_time,":/lcy/image/time.png");
    twoline->addWidget(ico_time);
    QLabel *lab_time = new QLabel;
    lab_time->setText(m_item.file_time);
    twoline->addWidget(lab_time);
    twoline->addStretch();

    QLabel* ico_film = new QLabel;
    setDefaultStyleSheet(ico_film,":/lcy/image/film.png");
    twoline->addWidget(ico_film);
    QLabel *lab_videoinfo = new QLabel;
    lab_videoinfo->setText(m_item.video_info);
    lab_videoinfo->setToolTip(m_item.video_info);
    twoline->addWidget(lab_videoinfo);


    QLabel* ico_audio = new QLabel;
    setDefaultStyleSheet(ico_audio,":/lcy/image/voice.png");
    twoline->addWidget(ico_audio);
    QLabel *lab_audio = new QLabel;
    lab_audio->setText(m_item.audio_info);
    lab_audio->setToolTip(m_item.audio_info);
    twoline->addWidget(lab_audio);

    return twoline;

}


void ItemView::setDefaultStyleSheet(QWidget *w, const QString &image)
{
    w->setStyleSheet("background-image: url("+image+");");
    QImage img(image);
    w->setFixedSize(img.size());
}

ItemView::~ItemView()
{
    this->deleteLater();

}

void ItemView::slot_DClickToPlay()
{

}

void ItemView::slot_destoryMySelf()
{
    emit parentDeleteMe(this);
}


void ItemView::removeItemLayout()
{
    QLayoutItem *p = main_layout->itemAtPosition(1,1);
    QHBoxLayout *tw = qobject_cast<QHBoxLayout*>(p->layout());
    while(tw->count())
    {
        QWidget *w = tw->takeAt(0)->widget();
        tw->removeWidget(w);
        delete w;
    }
    main_layout->removeItem(p);
    main_layout->update();

}

void ItemView::slot_MouseOnConvert()
{
    if(!m_item.isStandby)
    {
        removeItemLayout();
        slot_ConvertToStandby();
    }

    m_Process = new QProcess;
    connect(m_Process,SIGNAL(finished(int)),SLOT(slot_ConvertFinished(int)));
    connect(m_Process,SIGNAL(readyReadStandardOutput()),SLOT(slot_ConvertingStandardOutput()));
    QStringList arg;
    ConvertCfg cfg = m_ToolBoxSettings->getConvertArgments();
    m_item.outputFullPath = cfg.OutputDir+"/"+m_item.filename+".avi";
//    mencoder.exe  -of lavf   -oac pcm -ovc lavc -lavcopts  \
//    acodec=pcm_u8:vcodec=mjpeg:abitrate=96:vbitrate=640:vpass=1:aspect=16/9 -ofps 15 \
//    -af lavcresample=16000:channels=2  -vf scale -zoom -xy 160 AlanSiegel_2010-low-zh-cn.mp4 -o ppp.avi
    arg << "-of" << "lavf" << "-oac" << cfg.AEncoder << "-ovc" << "lavc" << "-lavcopts"
        << "acodec=pcm_u8:vcodec="+cfg.VEncoder+":abitrate="+cfg.ABitRate+":vbitrate="+cfg.VBitRate+":vpass="
           +cfg.EncoderCount+":aspect="+cfg.HWRatio << "-ofps" << cfg.Frame << "-af"
        << "lavcresample="+cfg.SampleRate+":channels="+cfg.Channel << "-vf" << "scale" << "-zoom"
        << "-xy" << cfg.Height << m_item.fullpath << "-o" << m_item.outputFullPath;
    m_Process->start(m_mencoder,arg);
    while(m_Process->waitForFinished(1000))
         QCoreApplication::processEvents();

}

void ItemView::slot_ConvertingStandardOutput()
{
    QByteArray ba(m_Process->readLine());

    if(ba.startsWith("Pos:"))
    {
       ba = ba.split('\r').at(0);
       ba = ba.split('(').at(1);
       ba = ba.split('%').at(0);
       QProgressBar *p = this->findChild<QProgressBar*>("pgbar");
       p->setValue(ba.toInt());
    }
}

void ItemView::slot_ConvertToStandby()
{
    QHBoxLayout* convert_lay = new QHBoxLayout;
    QProgressBar *pgbar = new QProgressBar;
    pgbar->setObjectName("pgbar");
    pgbar->setFixedHeight(12);
    pgbar->setAlignment(Qt::AlignCenter);
    pgbar->setRange(0,100);


    QPushButton *btn_stop = new QPushButton;
    connect(btn_stop,SIGNAL(clicked()),SLOT(slot_stopConvert()));
    btn_stop->setFixedSize(25,19);
    btn_stop->setIconSize(btn_stop->size());
    btn_stop->setIcon(QPixmap(":/lcy/image/stop.png").copy(0,19,25,19));
    btn_stop->setToolTip("取消转换");
    convert_lay->addWidget(pgbar);
    convert_lay->addWidget(btn_stop);
    main_layout->addLayout(convert_lay,1,1);
    m_item.isStandby = true;
}




void ItemView::slot_ConvertFinished(int)
{
    slot_stopConvert();
    m_item.isConverted = true;
//    QPalette p;
//    QPixmap img(QPixmap(m_item.image).scaled(68,54));
//    QBitmap mask(":/lcy/image/converted.png");
//    p.setBrush(QPalette::Window,QBrush(img));
//    lab_view->setPalette(p);
//    lab_view->setMask(mask);
//    lab_view->setWindowFlags(Qt::FramelessWindowHint);
    lab_view->setPixmap(QPixmap(":/lcy/image/converted.png"));
    removeItemLayout();
    m_Process->deleteLater();
    main_layout->addLayout(CreateItemInfoLayout(),1,1);


}

void ItemView::slot_stopConvert()
{
    m_Process->kill();
    m_Process->deleteLater();
    removeItemLayout();
    main_layout->addLayout(CreateItemInfoLayout(),1,1);
}

