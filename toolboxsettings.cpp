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
      cbox_enoderCount(new QComboBox),
      cbbox_aencoder(new QComboBox),
      cbbox_abitrate(new QComboBox),
      cbbox_channel(new QComboBox),
      cbbox_samplerate(new QComboBox),
      m_sys(new SystemSettings),
      edt_high(new QLineEdit),
      edt_width(new QLineEdit),
      twi_time(new QTableWidgetItem("00:00:00")),
      tedit_start(new QTimeEdit()),
      tedit_end(new   QTimeEdit),
      edt_fname(new QLineEdit())
{
    QGroupBox *main_gbox = new QGroupBox();
    main_ToolBox = new QToolBox(main_gbox);
    m_listComboBox << cbox_enoderCount << cbox_frameRate << cbox_hwrate << cbox_vbitrate
                      << cbox_vencoder << cbbox_abitrate << cbbox_aencoder <<cbbox_channel
                         << cbbox_samplerate;
    QStringList tmplist = m_sys->getlistAllItems();
    for(int i = 0 ; i < tmplist.count();i++)
    {
        m_listComboBox[i]->addItems(tmplist[i].split(','));
        connect(m_listComboBox[i],SIGNAL(currentTextChanged(QString)),
                SLOT(SomeValueHasChanged(QString)));
    }
    setFilmHW(m_sys->getFilmHW());

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


}

void ToolBoxSettings::SomeValueHasChanged(QString)
{
    updateStructConvertCfg();
}

void ToolBoxSettings::setFilmHW(QPair<QString, QString> pair)
{
    edt_high->setText(pair.first);
    edt_width->setText(pair.second);
}

void ToolBoxSettings::setBaseSettings()
{
    QLabel *lab_fname = new QLabel("文件名");
    edt_fname->setReadOnly(true);
    QLabel *lab_vsize = new QLabel("视频大小");
    QHBoxLayout *lay_hw = new QHBoxLayout;
    QLabel *lab_high = new QLabel("高:");

    edt_high->setInputMask("0000");
    edt_high->setFixedWidth(40);
    connect(edt_high,SIGNAL(textChanged(QString)),SLOT(SomeValueHasChanged(QString)));
    QLabel *Lab_width = new QLabel("宽:");
    edt_width->setInputMask("0000");
    edt_width->setFixedWidth(40);
    connect(edt_width,SIGNAL(textChanged(QString)),SLOT(SomeValueHasChanged(QString)));
    lay_hw->addWidget(lab_high);
    lay_hw->addWidget(edt_high);
    lay_hw->addWidget(Lab_width);
    lay_hw->addWidget(edt_width);

//    lay_hw->addStretch();
    QLabel *lab_quality = new QLabel("质量");
    QComboBox *cbox_quality = new QComboBox;
    QLabel *lab_totaltime = new QLabel("总时间长度");
//    QLineEdit *edt_time = new QLineEdit();






    base_Layout->addWidget(lab_fname);
    base_Layout->addWidget(edt_fname);
    base_Layout->addWidget(lab_vsize);
    base_Layout->addLayout(lay_hw);
    base_Layout->addWidget(lab_quality);
    base_Layout->addWidget(cbox_quality);
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

     twi_time->setText(pair.first.section(".",0,0));
     tedit_end->setTime(QTime::fromString(twi_time->text(),"HH:mm:ss"));
     edt_fname->setText(pair.second);
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
    video_Layout->addWidget(cbox_enoderCount);
    video_Layout->addWidget(lab_scale);
    video_Layout->addWidget(cbox_hwrate);
    QWidget *w = new QWidget;
    w->setLayout(video_Layout);
    main_ToolBox->addItem(w,QIcon(":/lcy/image/video_settings.png"),"视频设置");
}

void ToolBoxSettings::updateToolBox(QStringList list)
{
    setFilmHW(qMakePair(list.at(0),list.at(1)));
    for(int i = 2;i < list.count() ;i++)
    {
        m_listComboBox[i-2]->setCurrentText(list[i]);
    }
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
    m_ConvertCfg.Height = edt_high->text();
    m_ConvertCfg.Width = edt_width->text();
    m_ConvertCfg.ABitRate = cbbox_abitrate->currentText();
    m_ConvertCfg.AEncoder = cbbox_aencoder->currentText();
    m_ConvertCfg.Channel = cbbox_channel->currentText();
    m_ConvertCfg.EncoderCount = cbox_enoderCount->currentText();
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
