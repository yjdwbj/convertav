#include "systemsettings.h"

const QStringList VEncoder(QStringList() <<"mjpeg");
const QStringList AEncoder(QStringList() << "pcm");
const QStringList VBitRate(QStringList() << "256" << "512" << "768" << "1000" << "1200" << "1500");
const QStringList ABitRate(QStringList() << "16" << "32" << "40" << "48" << "56" << "64" << "80" << "96" << "112" << "128" <<"160" );
const QStringList FrameRate(QStringList() << "8" << "10" << "12" << "15" << "23.976" << "24" << "25" << "29.97" << "30");
const QStringList HWRatio(QStringList() << "1" << "4/3" << "16/9" << "221/100");
const QStringList EncoderCount(QStringList() << "1" << "2");
const QStringList SampleRate(QStringList() << "8000" << "22050" << "24000" << "32000" << "44100" << "48000");
const QStringList Channel(QStringList() << "1" << "2");



SystemSettings::SystemSettings(QWidget *parent)
    :QDialog(parent),
      box_output(new QGroupBox("设置输出目录")),
      box_audio(new QGroupBox("音频设置")),
      box_video(new QGroupBox("视频设置")),
      main_Layout(new QVBoxLayout)

{


    m_listAllItems << EncoderCount.join('|') << FrameRate.join('|') << HWRatio.join('|')
                   << VBitRate.join('|') << VEncoder.join('|') << ABitRate.join('|')
                   << AEncoder.join('|') << Channel.join('|') << SampleRate.join('|');
//    m_listComboBox << cbox_enoderCount << cbox_frameRate << cbox_hwrate << cbox_vbitrate
//                      << cbox_vencoder << cbbox_abitrate << cbbox_aencoder <<cbbox_channel
//                         << cbbox_samplerate;
    this->setWindowTitle("配置");
    QGridLayout *output_lay = new QGridLayout(box_output);
    edt_dir = new QLineEdit(box_output);
    edt_dir->setFixedWidth(400);
    edt_dir->setEnabled(false);
    setOutputDir(QDir::homePath()+"/My Documents/ConvertToMJPEG");

    QPushButton* btn_browser = new QPushButton("浏览目录",box_output);
    connect(btn_browser,SIGNAL(clicked()),SLOT(slot_setNewOutputDir()));

    QPushButton* btn_open = new QPushButton("打开目录",box_output);
    connect(btn_open,SIGNAL(clicked()),SLOT(slot_openOutputDir()));

    cbox_autoopen= new QCheckBox(box_output);
    cbox_autoopen->setFixedWidth(15);
    cbox_autoopen->setChecked(true);
    QLabel* lab_autoopen = new QLabel("转换完成后打开目录",box_output);

    output_lay->addWidget(edt_dir,0,0,1,2);
    output_lay->addWidget(btn_browser,0,2);
    output_lay->addWidget(btn_open,0,3);
    output_lay->addWidget(cbox_autoopen,1,0);
    output_lay->addWidget(lab_autoopen,1,1,Qt::AlignLeft);


    QHBoxLayout *hw_layout = new QHBoxLayout();
    QGridLayout *video_lay = new QGridLayout(box_video);
    QLabel *lab_height = new QLabel("高:",box_video);
    lab_height->setFixedWidth(24);
    QLabel *lab_width = new QLabel("宽:",box_video);
    lab_width->setFixedWidth(24);


    edt_height = new QLineEdit(box_video);
    edt_height->setFixedWidth(50);
    edt_width = new QLineEdit(box_video);
    edt_width->setFixedWidth(50);

//    hw_layout->setSpacing(1);
    hw_layout->addWidget(lab_height,Qt::AlignCenter);
    hw_layout->addWidget(edt_height);
    hw_layout->addWidget(lab_width,Qt::AlignCenter);
    hw_layout->addWidget(edt_width);


    QLabel* lab_vencoder = new QLabel("编码:",box_video);
    cbox_vencoder = new QComboBox(box_video);

    QLabel* lab_hwrate = new QLabel("高宽比:",box_video);
    cbox_hwrate = new QComboBox(box_video);

    QLabel* lab_vbitrate = new QLabel("比特率:",box_video);
    cbox_vbitrate = new QComboBox(box_video);

    QLabel* lab_enoderCount = new QLabel("编码遍数:",box_video);
    cbox_enoderCount = new QComboBox(box_video);

    QLabel* lab_frameRate = new QLabel("帧数:",box_video);
    cbox_frameRate = new QComboBox(box_video);

//    video_lay->setSpacing(10);
    video_lay->addLayout(hw_layout,0,0,1,2);
    video_lay->addWidget(lab_vencoder,0,2,Qt::AlignCenter);
    video_lay->addWidget(cbox_vencoder,0,3);
    video_lay->addWidget(lab_hwrate,0,4,Qt::AlignCenter);
    video_lay->addWidget(cbox_hwrate,0,5);
    video_lay->addWidget(lab_vbitrate,1,0,Qt::AlignCenter);
    video_lay->addWidget(cbox_vbitrate,1,1);
    video_lay->addWidget(lab_enoderCount,1,2,Qt::AlignCenter);
    video_lay->addWidget(cbox_enoderCount,1,3);
    video_lay->addWidget(lab_frameRate,1,4,Qt::AlignCenter);
    video_lay->addWidget(cbox_frameRate,1,5);


    QGridLayout *audio_layout = new QGridLayout(box_audio);

    QLabel* lab_aencoder = new QLabel("音频编码:",box_audio);
    cbbox_aencoder = new QComboBox(box_audio);

    QLabel* lab_abitrate = new QLabel("比特率:",box_audio);
    cbbox_abitrate = new QComboBox(box_audio);

    QLabel* lab_channel = new QLabel("声道:",box_audio);
     cbbox_channel = new QComboBox();

    QLabel* lab_samplerate = new QLabel("采样率:",box_audio);
    cbbox_samplerate = new QComboBox;

    audio_layout->addWidget(lab_aencoder,0,0,Qt::AlignCenter);
    audio_layout->addWidget(cbbox_aencoder,0,1);
    audio_layout->addWidget(lab_abitrate,0,2,Qt::AlignCenter);
    audio_layout->addWidget(cbbox_abitrate,0,3);
    audio_layout->addWidget(lab_channel,1,0,Qt::AlignCenter);
    audio_layout->addWidget(cbbox_channel,1,1);
    audio_layout->addWidget(lab_samplerate,1,2,Qt::AlignCenter);
    audio_layout->addWidget(cbbox_samplerate,1,3);



    main_Layout->addWidget(box_output);
    main_Layout->addWidget(box_video);
    main_Layout->addWidget(box_audio);


    QPushButton* btn_save = new QPushButton("保存");
    btn_save->setObjectName("btn_save");
    btn_save->setFixedSize(194,45);
    connect(btn_save,SIGNAL(clicked()),SLOT(accept()));

    QPushButton* btn_cancel = new QPushButton("取消");
    btn_cancel->setObjectName("btn_cancel");
    btn_cancel->setFixedSize(194,45);
    connect(btn_cancel,SIGNAL(clicked()),SLOT(reject()));

    QHBoxLayout *btn_layout = new QHBoxLayout;
    btn_layout->addWidget(btn_save);
    btn_layout->addWidget(btn_cancel);

    main_Layout->addLayout(btn_layout,Qt::AlignCenter);


    this->setLayout(main_Layout);


    InitDialog();
}


void SystemSettings::slot_openOutputDir()
{
     QDesktopServices::openUrl(QUrl(tr("file:///")+edt_dir->text()));
}

void SystemSettings::slot_setNewOutputDir()
{
    QString filename = QFileDialog::getExistingDirectory(this,"选择输出目录",edt_dir->text());
    if(!filename.isEmpty())
        setOutputDir(filename);
}

void SystemSettings::InitDialog()
{



    setVideoSize(128,160);
    cbox_enoderCount->addItems(EncoderCount);
    cbox_frameRate->addItems(FrameRate);
    cbox_hwrate->addItems(HWRatio);
    cbox_vbitrate->addItems(VBitRate);
    cbox_vencoder->addItems(VEncoder);
    cbbox_abitrate->addItems(ABitRate);
    cbbox_aencoder->addItems(AEncoder);
    cbbox_channel->addItems(Channel);
    cbbox_samplerate->addItems(SampleRate);
}

QStringList SystemSettings::getCurrentIndexText()
{
    m_currentIndexText.clear();
    m_currentIndexText << edt_height->text() << edt_width->text() << cbox_enoderCount->currentText()
    << cbox_frameRate->currentText() << cbox_hwrate->currentText() << cbox_vbitrate->currentText()
    << cbox_vencoder->currentText() << cbbox_abitrate->currentText() << cbbox_aencoder->currentText()
    <<   cbbox_channel->currentText() << cbbox_samplerate->currentText();
    return m_currentIndexText;

}

SystemSettings::~SystemSettings()
{

}


void SystemSettings::setDefaultCfg()
{

}
