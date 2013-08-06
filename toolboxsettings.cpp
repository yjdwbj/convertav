#include "toolboxsettings.h"

ToolBoxSettings::ToolBoxSettings(QWidget *parent)
    :QWidget(parent),
      base_Layout(new QVBoxLayout),
      audio_Layout(new QVBoxLayout),
      video_Layout(new QVBoxLayout)

{
    QGroupBox *main_gbox = new QGroupBox();
    main_ToolBox = new QToolBox(main_gbox);
    setBaseSettings();
    setAudioSettings();
    setVideoSettings();

    QVBoxLayout *vlayout = new QVBoxLayout(main_gbox);
    vlayout->setSpacing(0);
    QVBoxLayout *main_Layout = new QVBoxLayout;
    vlayout->addWidget(main_ToolBox);
    main_Layout->addWidget(main_gbox);
    main_Layout->setSpacing(0);
    setLayout(main_Layout);


}


void ToolBoxSettings::setBaseSettings()
{
    QLabel *lab_fname = new QLabel("文件名");
    QLineEdit *edt_fname = new QLineEdit();
    QLabel *lab_vsize = new QLabel("视频大小");
    QComboBox *cbox_vsize = new QComboBox();
    QLabel *lab_quality = new QLabel("质量");
    QComboBox *cbox_quality = new QComboBox;
    QLabel *lab_totaltime = new QLabel("总时间");
    QLineEdit *edt_time = new QLineEdit();

    QList<QWidget*> wlist;
    wlist << lab_fname << edt_fname << lab_vsize << cbox_vsize << lab_quality << cbox_quality << lab_totaltime << edt_time;
    for(int i = 0; i < wlist.size();i++)
    {
        base_Layout->addWidget(wlist.at(i));
    }
    QWidget *w = new QWidget;
    w->setLayout(base_Layout);
    main_ToolBox->addItem(w,"基本设置");
}

void ToolBoxSettings::setAudioSettings()
{


}

void ToolBoxSettings::setVideoSettings()
{
    QLabel *lab_encoder = new QLabel("视频编码");
    QComboBox *cbox_encoder = new QComboBox;
    QLabel *lab_frameRate = new QLabel("帧率");
    QComboBox *cbox_frameRate = new QComboBox;
    QLabel *lab_scale = new QLabel("高宽比");
    QComboBox *cbox_scale = new QComboBox;
    video_Layout->addWidget(lab_encoder);
    video_Layout->addWidget(cbox_encoder);
    video_Layout->addWidget(lab_frameRate);
    video_Layout->addWidget(cbox_frameRate);
    video_Layout->addWidget(lab_scale);
    video_Layout->addWidget(cbox_scale);
    QWidget *w = new QWidget;
    w->setLayout(video_Layout);
    main_ToolBox->addItem(w,"视频设置");
}

ToolBoxSettings::~ToolBoxSettings()
{

}
