#include "toolboxsettings.h"
#include <QHeaderView>

ToolBoxSettings::ToolBoxSettings(QWidget *parent)
    :QWidget(parent),
      base_Layout(new QVBoxLayout),
      audio_Layout(new QVBoxLayout),
      video_Layout(new QVBoxLayout),
      cbox_vencoder(new QComboBox),
      cbox_frameRate(new QComboBox),
      cbox_hwrate(new QComboBox),
      cbox_vbitrate(new QComboBox),
      cbox_encoderCount(new QComboBox),
      cbbox_aencoder(new QComboBox),
      cbbox_abitrate(new QComboBox),
      cbbox_channel(new QComboBox),
      cbbox_samplerate(new QComboBox),
      cbbox_scale(new QComboBox),
      m_sys(new SystemSettings),
//      edt_high(new QLineEdit),
//      edt_width(new QLineEdit),

      twi_time(new QTableWidgetItem("00:00:00")),
      tedit_start(new QTimeEdit()),
      tedit_end(new   QTimeEdit),
      edt_fname(new QLineEdit())
{

    QGroupBox *main_gbox = new QGroupBox();
    main_ToolBox = new QToolBox(main_gbox);
    m_ToolBoxlistComboBox  << cbox_encoderCount << cbox_frameRate << cbox_hwrate << cbox_vbitrate
                      << cbox_vencoder << cbbox_abitrate << cbbox_aencoder <<cbbox_channel
                         << cbbox_samplerate << cbbox_scale;
   // m_listComboBox = m_sys->getAllComboBox();
    m_SystemVauleList = m_sys->getlistAllItems();
    int i = 0;
    for(i = 0 ; i < m_SystemVauleList.count();i++)
    {
        m_ToolBoxlistComboBox[i]->addItems(m_SystemVauleList[i].split(','));
        connect(m_ToolBoxlistComboBox[i],SIGNAL(currentTextChanged(QString)),
                SLOT(SomeValueHasChanged(QString)));
    }

    cbbox_abitrate->setEnabled(false); // pcm_* not use the audio bitrate
    cbbox_abitrate->clear();
    cbox_vbitrate->setEnabled(false);

   // setFilmHW(m_SystemVauleList.last());

    setBaseSettings();
    setAudioSettings();
    setVideoSettings();
    updateStructConvertCfg();

    QVBoxLayout *vlayout = new QVBoxLayout(main_gbox);
    vlayout->setSpacing(0);
    QVBoxLayout *main_Layout = new QVBoxLayout;
    vlayout->addWidget(main_ToolBox);
    main_Layout->addWidget(main_gbox);
    main_Layout->setSpacing(0);
    setLayout(main_Layout);
    this->setEnabled(false);
}

void ToolBoxSettings::SomeValueHasChanged(QString)
{
    updateStructConvertCfg(); // 更改了一个某一个值
}

void ToolBoxSettings::setFilmHW(const QString &s)
{
//    edt_high->setText(pair.first);
//    edt_width->setText(pair.second);
    cbbox_scale->setCurrentText(s);
}

void ToolBoxSettings::setBaseSettings()
{
    QLabel *lab_fname = new QLabel("文件名");
    edt_fname->setReadOnly(true);
    edt_fname->setEnabled(false);
    QLabel *lab_vsize = new QLabel("视频大小");
    QHBoxLayout *lay_hw = new QHBoxLayout;

    lay_hw->addWidget(cbbox_scale);


//    QLabel *lab_quality = new QLabel("质量");
//    QComboBox *cbox_quality = new QComboBox;
    QLabel *lab_totaltime = new QLabel("总时间长度");


    base_Layout->addWidget(lab_fname);
    base_Layout->addWidget(edt_fname);
    base_Layout->addWidget(lab_vsize);
    base_Layout->addLayout(lay_hw);
//    base_Layout->addWidget(lab_quality);
//    base_Layout->addWidget(cbox_quality);
    base_Layout->addWidget(lab_totaltime);
    CreateTotalTimeLayout();
    base_Layout->addWidget(tw_time);


    QWidget *w = new QWidget;
    w->setLayout(base_Layout);
    main_ToolBox->addItem(w,QIcon(":/lcy/image/basic_settings.png"),"基本设置");
}


void ToolBoxSettings::CreateTotalTimeLayout()
{

    tw_time = new QTableWidget(3,2);

    tw_time->setGridStyle(Qt::SolidLine);
    tw_time->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tw_time->setShowGrid(true);
    tw_time->setColumnWidth(0,80);
    tw_time->horizontalHeader()->setStretchLastSection(true);

    tw_time->verticalHeader()->setVisible(false);
    tw_time->horizontalHeader()->setVisible(false);
    tw_time->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tw_time->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tw_time->setSpan(0,0,1,2);
//    tw->resizeColumnsToContents();
    tw_time->resizeRowsToContents();
//    tw->resizeColumnToContents(0);


    tw_time->setItem(0,0,twi_time);

    twi_start = new QTableWidgetItem("转换起始点");
    twi_end = new QTableWidgetItem("转换终止点");

    tw_time->setItem(1,0,twi_start);
    tw_time->setItem(2,0,twi_end);

    tedit_start->setDisplayFormat("HH:mm:ss");
    tedit_start->setTime(QTime(0,0,0));
    tw_time->setCellWidget(1,1,tedit_start);


    tedit_end->setObjectName("endTime");
    tedit_end->setDisplayFormat("HH:mm:ss");
    tw_time->setCellWidget(2,1,tedit_end);
    connect(tedit_start,SIGNAL(editingFinished()),SLOT(slot_updateTime()));
    connect(tedit_end,SIGNAL(editingFinished()),SLOT(slot_updateTime()));

}


void ToolBoxSettings::slot_updateTime()
{
    updateStructConvertCfg();
}

void ToolBoxSettings::setTimeAndNameToTable(const QPair<QString,QString> &pair)
{

     this->setEnabled(true);
    QString filetime = pair.first.section(".",0,0);
     twi_time->setText(filetime);
#ifndef QT_NO_DEBUG

#endif

     tedit_start->setMaximumTime(QTime::fromString(filetime,"HH:mm:ss"));
     tedit_end->setMaximumTime(QTime::fromString(filetime,"HH:mm:ss"));
     tedit_start->setTime(QTime(0,0,0,0));
     tedit_end->setTime(QTime::fromString(twi_time->text(),"HH:mm:ss"));
     edt_fname->setText(pair.second);
     m_sys->UpdateCurrentIndexText();
     updateToolBox(m_sys->getCurrentIndexText());
     updateStructConvertCfg();
}

void ToolBoxSettings::setVideoSettings()
{
    QLabel *lab_encoder = new QLabel("视频编码");
    QLabel *lab_frameRate = new QLabel("帧率");
    QLabel *lab_bitRate = new QLabel("比特率");
    QLabel *lab_encoderCount = new QLabel("编码遍数");
    QLabel *lab_scale = new QLabel("高宽比");

    video_Layout->addWidget(lab_encoder);
    video_Layout->addWidget(cbox_vencoder);
    video_Layout->addWidget(lab_frameRate);
    video_Layout->addWidget(cbox_frameRate);
    video_Layout->addWidget(lab_bitRate);
    video_Layout->addWidget(cbox_vbitrate);
    video_Layout->addWidget(lab_encoderCount);
    video_Layout->addWidget(cbox_encoderCount);
    video_Layout->addWidget(lab_scale);
    video_Layout->addWidget(cbox_hwrate);
    QWidget *w = new QWidget;
    w->setLayout(video_Layout);
    main_ToolBox->addItem(w,QIcon(":/lcy/image/video_settings.png"),"视频设置");
}

void ToolBoxSettings::updateToolBox(QStringList list)
{
   setFilmHW(list.last());
    int i = 0;
    for(;i < list.count() ;i++)
    {
        m_ToolBoxlistComboBox[i]->setCurrentText(list[i]);
    }

}

void ToolBoxSettings::clearAllInput()
{
    int i = 0;
    for(;i < m_ToolBoxlistComboBox.count() ;i++)
    {
        m_ToolBoxlistComboBox[i]->clear();
    }
    edt_fname->setText("");
    twi_time->setText("");
    twi_end->setText("");
}

void ToolBoxSettings::setAudioSettings()
{
    QLabel *lab_encoder = new QLabel("音频编码");
    QLabel *lab_bitRate = new QLabel("比特率");
    QLabel *lab_channel = new QLabel("声道");
    QLabel *lab_sampleRate = new QLabel("采样率");


    audio_Layout->addWidget(lab_encoder);
    audio_Layout->addWidget(cbbox_aencoder);
    audio_Layout->addWidget(lab_bitRate);
    audio_Layout->addWidget(cbbox_abitrate);
    audio_Layout->addWidget(lab_channel);
    audio_Layout->addWidget(cbbox_channel);
    audio_Layout->addWidget(lab_sampleRate);
    audio_Layout->addWidget(cbbox_samplerate);
    QWidget *w = new QWidget;
    w->setLayout(audio_Layout);
    main_ToolBox->addItem(w,QIcon(":/lcy/image/audio_settings.png"),"音频设置");
}


void ToolBoxSettings::updateStructConvertCfg()
{
    m_ConvertCfg.OutputDir = m_sys->getOutputDir();
//    m_ConvertCfg.Height = edt_high->text();
//    m_ConvertCfg.Width = edt_width->text();
    m_ConvertCfg.VScale = cbbox_scale->currentText().section(" ",1,1);
//  m_ConvertCfg.ABitRate = cbbox_abitrate->currentText();
    m_ConvertCfg.AEncoder = cbbox_aencoder->currentText();
    m_ConvertCfg.Channel = cbbox_channel->currentText();
    m_ConvertCfg.EncoderCount = cbox_encoderCount->currentText();
    m_ConvertCfg.Frame = cbox_frameRate->currentText();
    m_ConvertCfg.HWRatio = cbox_hwrate->currentText();
    m_ConvertCfg.SampleRate = cbbox_samplerate->currentText();
    m_ConvertCfg.VBitRate = cbox_vbitrate->currentText();
    m_ConvertCfg.VEncoder = cbox_vencoder->currentText();
    m_ConvertCfg.StartTime = tedit_start->text();
    m_ConvertCfg.Endpos= tedit_end->text();

}

ToolBoxSettings::~ToolBoxSettings()
{

}
