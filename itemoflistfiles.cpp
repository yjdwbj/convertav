#include "itemoflistfiles.h"
#include <QFrame>




ItemView::ItemView(itemstruct &item, const ToolBoxSettings *tbs, QWidget *parent)
    :QTabWidget(parent),
      main_layout(new QGridLayout),
//      btn_convert(new QPushButton),
//      btn_delself(new QPushButton),
//      lab_audio(new QLabel),
//      lab_videoinfo(new QLabel),
      lab_view(new QLabel("test")),
      m_AppPath(qApp->applicationDirPath()),
//      m_mencoder(m_AppPath+ ("/mencoder.exe")),
      m_ffmpeg(m_AppPath + ("/ffmpeg.exe")),
//      cbox_selelct(new QCheckBox),
      _state(Stopped),
      _checked(Qt::Checked)
{
     m_item = item;
     m_item.isSingleConvert = false;
     m_ToolBoxSettings = tbs;
    this->setStyleSheet("QProgressBar {"
                        "background-image: url(:/lcy/image/progress_bk.png);}"
                        "QProgressBar::chunk {"
                        "background-image: url(:/lcy/image/progress_fg.png);}");

    lab_view->setFrameShape(QFrame::Panel);
    QImage img(m_item.image);

    lab_view->setFixedSize(68,54);
    lab_view->setPixmap(QPixmap::fromImage(img).scaled(68,54));

    main_layout->addWidget(lab_view,0,0,2,1);
    removeTempFile(m_item.image);




//    main_layout->addWidget(btn_convert);

    main_layout->addLayout(CreateFirstLine(),0,1);
    main_layout->addLayout(CreateItemInfoLayout(),1,1);
    main_layout->setSpacing(5);
    QWidget *w=new QWidget;
    w->setLayout(main_layout);
    addTab(w,item.filename);
    this->setToolTip(item.filename);
    fullpath = item.fullpath;

}

QLayout* ItemView::CreateFirstLine()
{
    QHBoxLayout *oneline = new QHBoxLayout;

    btn_rename = new QPushButton(QPixmap(":/lcy/image/pen.png").copy(0,0,14,14),"");
    btn_rename->setToolTip("重命名");
    btn_rename->setFixedSize(16,16);
    oneline->addWidget(btn_rename);
    connect(btn_rename,SIGNAL(clicked()),SLOT(slot_ReNameFile()));

    QLabel* oname = new QLabel(m_item.filename);

    oneline->addWidget(oname);

//    oneline->addStretch(10);
    btn_convert = new QPushButton(QPixmap(":/lcy/image/converter.png").copy(0,37,25,19),"");
    btn_convert->setToolTip(tr("开始转换"));
    btn_convert->setFixedSize(25,19);
    btn_convert->setIconSize(btn_convert->size());
//    btn_convert->setIcon(QPixmap(":/lcy/image/converter.png").copy(0,37,25,19));
    btn_convert->setObjectName("btn_convert");
    connect(btn_convert,SIGNAL(pressed()),SLOT(slot_SingleConvert()));
    connect(this,SIGNAL(parentConvertSignal()),SLOT(slot_MouseOnConvertFFMPEG()));
    oneline->addWidget(btn_convert);

    btn_delself = new QPushButton(QPixmap(":/lcy/image/no.png").copy(0,0,39,19),"");
    btn_delself->setFixedSize(39,19);
    btn_delself->setIconSize(btn_delself->size());
//    btn_delself->setIcon(QPixmap(":/lcy/image/no.png").copy(0,0,39,19));
    btn_delself->setToolTip("删除该项");
    connect(btn_delself,SIGNAL(clicked()),SLOT(slot_destoryMySelf()));

    oneline->addWidget(btn_delself);
    return oneline;
}


void ItemView::slot_SingleConvert()
{
    m_item.isStandby = false;
    m_item.isSingleConvert = true;
    m_item.isCancelConvert = false;
    m_item.isConverted = false;
    slot_MouseOnConvertFFMPEG();
}

void ItemView::slot_ReNameFile()
{
    removeItemLayout(main_layout->itemAtPosition(0,1));
    main_layout->addLayout(CreateReNameLayout(),0,1);
}

QLayout* ItemView::CreateReNameLayout()
{
    QHBoxLayout *oneline = new QHBoxLayout;
    btn_rename = new QPushButton(QPixmap(":/lcy/image/pen.png").copy(0,0,14,14),"");
    btn_rename->setFixedSize(16,16);
    btn_rename->setToolTip("重命名");

    edt_rename = new QLineEdit;
    edt_rename->setObjectName("ReName");
    edt_rename->setText(m_item.filename);
//    connect(edt_rename,SIGNAL(editingFinished()),SLOT(slot_EditOutOfFoucs()));

    btn_rename_accpet = new QPushButton(QPixmap(":/lcy/image/sure.png").copy(0,0,10,11),"");
    connect(btn_rename_accpet,SIGNAL(clicked()),SLOT(slot_SureReNameFile()));
    btn_rename_accpet->setFixedSize(16,16);
    btn_rename_accpet->setIconSize(btn_rename_accpet->size());
    btn_rename_accpet->setToolTip("确定");


    btn_rename_reject = new QPushButton(QPixmap(":/lcy/image/cancel.png").copy(0,0,10,11),"");
    connect(btn_rename_reject,SIGNAL(clicked()),SLOT(slot_CancelReNameFile()));
    btn_rename_reject->setFixedSize(16,16);
    btn_rename_reject->setIconSize(btn_rename_reject->size());
    btn_rename_reject->setToolTip("取消");

    oneline->addWidget(btn_rename);
    oneline->addWidget(edt_rename);
    oneline->addWidget(btn_rename_accpet);
    oneline->addWidget(btn_rename_reject);
    return oneline;


}

QLayout* ItemView::CreateItemInfoLayout()
{
//    cbox_selelct->setCheckState(Qt::Checked);
    QHBoxLayout *twoline = new QHBoxLayout;
    QLabel* ico_time = new QLabel;
    setDefaultStyleSheet(ico_time,":/lcy/image/time.png");
//    twoline->addWidget(cbox_selelct);
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

void ItemView::slot_SureReNameFile()
{
    m_item.filename = edt_rename->text();
    removeItemLayout(main_layout->itemAtPosition(0,1));
    main_layout->addLayout(CreateFirstLine(),0,1);
}

void ItemView::slot_EditOutOfFoucs()
{
    removeItemLayout(main_layout->itemAtPosition(0,1));
    main_layout->addLayout(CreateFirstLine(),0,1);
}

void ItemView::slot_CancelReNameFile()
{
    removeItemLayout(main_layout->itemAtPosition(0,1));
    main_layout->addLayout(CreateFirstLine(),0,1);
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


void ItemView::slot_destoryMySelf()
{
    emit parentDeleteMe(this);
}


void ItemView::removeItemLayout(QLayoutItem *p)
{

    QHBoxLayout *tw = qobject_cast<QHBoxLayout*>(p->layout());
    if(!tw)
        return ;
    while(tw->count())
    {
        QWidget *w = tw->takeAt(0)->widget();
        if(w)
        {
            tw->removeWidget(w);
            delete w;
        }

    }
    main_layout->removeItem(p);
    main_layout->update();

}

void ItemView::slot_MouseOnConvertFFMPEG()
{
    if(m_item.isCancelConvert)
        return;
    if(!m_item.isStandby)
    {
        removeItemLayout(main_layout->itemAtPosition(1,1));
        main_layout->addLayout(CreateConvertingLayout(),1,1);
    }

    m_Process = new QProcess;
    connect(m_Process,SIGNAL(finished(int)),SLOT(slot_ConvertFinished(int)));
    connect(m_Process,SIGNAL(readyReadStandardOutput()),SLOT(slot_ConvertingStandardOutput()));
    connect(m_Process,SIGNAL(readyReadStandardError()),SLOT(slot_ConvertingStandardOutput()));
    QStringList arg;
    ConvertCfg cfg = m_ToolBoxSettings->getConvertArgments();
    m_item.outputFullPath = cfg.OutputDir+"/"+m_item.filename+".avi";

    QTime tt(0,0,0);
     m_fileLengthForSeconds = tt.secsTo(QTime::fromString(cfg.Endpos,"HH:mm:ss"));


    arg <<  "-i" << m_item.fullpath << "-ss" << cfg.StartTime << "-t" << cfg.Endpos  << "-c:a" << cfg.AEncoder << "-c:v" << cfg.VEncoder
//         << "-b:a" << cfg.ABitRate // here is valid
//         << "-b:v"<< cfg.VBitRate
         << /*"-pass" << cfg.EncoderCount <<*/ "-aspect"
         << cfg.HWRatio << "-r" << cfg.Frame << "-ar"
        << cfg.SampleRate << "-ac" << cfg.Channel << "-y" << "-s" << cfg.VScale   <<  m_item.outputFullPath;

    _state = Converting;
    this->btn_delself->setEnabled(false);
    m_Process->start(m_ffmpeg,arg);
    if(!m_Process->waitForStarted())
        return;
//    while(!m_Process->waitForFinished(500))
//        QApplication::processEvents();
}



void ItemView::slot_ConvertingStandardOutput()
{
//    QByteArray ba(m_Process->readLine());

//    if(_state == Stopped)
//        return;
//    if(ba.startsWith("Pos:"))
//    {
//       ba = ba.split('\r').at(0);
//       ba = ba.split('(').at(1);
//       ba = ba.split('%').at(0);
//       QProgressBar *p = this->findChild<QProgressBar*>("pgbar");
//       if(p)
//       p->setValue(ba.toInt());
//    }

    QString out(m_Process->readAllStandardError());
    QStringList outlist = out.split(QRegExp("(\\r\\n)"),
                                    QString::SkipEmptyParts);
       if(_state == Stopped)
            return;
       for(int i =0 ; i < outlist.size();i++)
       if(outlist[i].startsWith("frame="))
       {
           QString tmp = out.section("=",5,5);
           tmp = tmp.section(".",0,0);
           QTime tt(0,0,0);
           float sec = tt.secsTo(QTime::fromString(tmp,"HH:mm:ss"));
           sec = (sec / m_fileLengthForSeconds) * 100;
           QProgressBar *p = this->findChild<QProgressBar*>("pgbar");
           if(p)
           p->setValue(sec);
       }
}

QLayout* ItemView::CreateConvertingLayout()
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
    return convert_lay;
}


void ItemView::slot_ConvertToStandby()
{
    removeItemLayout(main_layout->itemAtPosition(1,1));
    main_layout->addLayout(CreateConvertingLayout(),1,1);
    m_item.isStandby = true;
    m_item.isCancelConvert = false;
}




void ItemView::slot_ConvertFinished(int ret)
{
  //  removeTempFile(m_item.outputFullPath+".bak");
    _state = Stopped;
    if(m_Process->exitStatus()==QProcess::NormalExit)
    {
        this->btn_delself->setEnabled(true);
        m_item.isConverted = true;
        lab_view->setPixmap(QPixmap(":/lcy/image/converted.png"));
    }
    if(m_item.isSingleConvert)
    {
        ConvertCfg cfg = m_ToolBoxSettings->getConvertArgments();
        if(cfg.AutoOpen && m_item.isConverted)
            QDesktopServices::openUrl(QUrl(tr("file:///")+cfg.OutputDir));
    }





    removeItemLayout(main_layout->itemAtPosition(1,1));
    main_layout->addLayout(CreateItemInfoLayout(),1,1);

}

void ItemView::removeTempFile(const QString &file)
{
    QFile::remove(file);
}

void ItemView::slot_stopConvert()
{
  //  removeTempFile(m_item.outputFullPath+".bak");
     this->btn_delself->setEnabled(true);
    if((getState() == Stopped)
            && m_item.isStandby)
    {
        m_item.isCancelConvert = true;
        removeItemLayout(main_layout->itemAtPosition(1,1));
        main_layout->addLayout(CreateItemInfoLayout(),1,1);
        return ;
    }
    m_Process->kill();
    m_Process->deleteLater();
    _state = Stopped;

    removeItemLayout(main_layout->itemAtPosition(1,1));
    main_layout->addLayout(CreateItemInfoLayout(),1,1);
}

