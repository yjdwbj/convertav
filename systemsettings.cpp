#include "systemsettings.h"

const QString VEncoder("mjpeg");
const QString AEncoder("pcm_u8,pcm_s16le");
const QString VBitRate("");
const QString ABitRate("" );
const QString FrameRate("8,10,12,15,23.976,24,25,29.97,30");
const QString HWRatio("1,4:3,16:9,221:100");
const QString EncoderCount("1,2");
const QString SampleRate("8000,22050,24000,32000,44100,48000");
const QString Channel("1,2");

const QStringList m_listkey(QStringList()  << "EnconderCount=" << "FrameRate=" << "HWRate=" << "VBitrate="
          << "VEncoder=" << "ABitrate=" << "AEncoder=" << "Channel=" << "Samplerate=" << "Scale=");

/* the m_listkey order must be set same m_listAllItems and m_listcbbox */

SystemSettings::SystemSettings(QWidget *parent)
    :QDialog(parent),
      box_output(new QGroupBox("设置输出目录")),
      box_audio(new QGroupBox("音频设置")),
      box_video(new QGroupBox("视频设置")),
      main_Layout(new QVBoxLayout)

{


    m_listAllItems << EncoderCount << FrameRate << HWRatio << VBitRate << VEncoder << ABitRate
                   << AEncoder << Channel << SampleRate << GetSupportedScale().join(",");
//    m_listComboBox << cbox_encoderCount << cbox_frameRate << cbox_hwrate << cbox_vbitrate
//                      << cbox_vencoder << cbbox_abitrate << cbbox_aencoder <<cbbox_channel
//                         << cbbox_samplerate;
    this->setWindowTitle("配置");
    this->setWindowIcon(QPixmap(":/lcy/image/option.png").copy(0,0,26,26));
    QGridLayout *output_lay = new QGridLayout(box_output);
    edt_dir = new QLineEdit(box_output);
    edt_dir->setFixedWidth(400);
    edt_dir->setEnabled(false);
    setOutputDir(QDir::homePath()+"/My Documents/ConvertToMJPEG");
    if(!QFileInfo(getOutputDir()).exists())
    {
        QDir(QDir::root()).mkdir(getOutputDir());
    }
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

    QLabel *lab_videoSize = new QLabel("视频尺寸:",box_video);
//    QLabel *lab_height = new QLabel("高:",box_video);
//    lab_height->setFixedWidth(24);
//    QLabel *lab_width = new QLabel("宽:",box_video);
//    lab_width->setFixedWidth(24);


//    edt_height = new QLineEdit(box_video);
//    edt_height->setFixedWidth(50);
//    edt_width = new QLineEdit(box_video);
//    edt_width->setFixedWidth(50);
    cbbox_scale = new QComboBox(box_video);
    hw_layout->setSpacing(1);
    hw_layout->addWidget(lab_videoSize,Qt::AlignCenter);
    hw_layout->addWidget(cbbox_scale);
//    hw_layout->addWidget(lab_width,Qt::AlignCenter);
//    hw_layout->addWidget(edt_width);


    QLabel* lab_vencoder = new QLabel("编码:",box_video);
    cbox_vencoder = new QComboBox(box_video);

    QLabel* lab_hwrate = new QLabel("高宽比:",box_video);
    cbox_hwrate = new QComboBox(box_video);

    QLabel* lab_vbitrate = new QLabel("比特率:",box_video);
    cbox_vbitrate = new QComboBox(box_video);
    cbox_vbitrate->setEnabled(false);

    QLabel* lab_enoderCount = new QLabel("编码遍数:",box_video);
    cbox_encoderCount = new QComboBox(box_video);

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
    video_lay->addWidget(cbox_encoderCount,1,3);
    video_lay->addWidget(lab_frameRate,1,4,Qt::AlignCenter);
    video_lay->addWidget(cbox_frameRate,1,5);


    QGridLayout *audio_layout = new QGridLayout(box_audio);

    QLabel* lab_aencoder = new QLabel("音频编码:",box_audio);
    cbbox_aencoder = new QComboBox(box_audio);

    QLabel* lab_abitrate = new QLabel("比特率:",box_audio);
    cbbox_abitrate = new QComboBox(box_audio);
    cbbox_abitrate->setEnabled(false);

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



    m_listcbbox << cbox_encoderCount << cbox_frameRate << cbox_hwrate << cbox_vbitrate << cbox_vencoder
                  << cbbox_abitrate << cbbox_aencoder << cbbox_channel << cbbox_samplerate << cbbox_scale;



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
    readCfgToFile(QDir::homePath()+"/Application Data/ConvertToMJPEG/convert_to_mjeg.ini");
}

QStringList SystemSettings::GetSupportedScale()
{
    int len = sizeof(support_scale)/sizeof(VideoSize);
    QStringList list;
    QString t;
    for(int i =0 ;i < len ;i++)
    {
        t = support_scale[i].name ;
        t.append(" ");
        t.append(support_scale[i].size) ;
        list.append( t);
    }
    return list;
}


void SystemSettings::FilesOrDirNoExists(const QString &in)
{
    QMessageBox::warning(this,"出错啦!","找不到这个文件："+in);
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



  //  setVideoSize(128,160);
    int i =0;
    for( i = 0 ; i < m_listAllItems.count();i++)
    {
        m_listcbbox[i]->addItems(m_listAllItems[i].split(","));
    }


}

void SystemSettings::UpdateCurrentIndexText()
{
    m_currentIndexText.clear();
    m_currentIndexText  << cbox_encoderCount->currentText()
    << cbox_frameRate->currentText() << cbox_hwrate->currentText() << cbox_vbitrate->currentText()
    << cbox_vencoder->currentText() << cbbox_abitrate->currentText() << cbbox_aencoder->currentText()
    <<   cbbox_channel->currentText() << cbbox_samplerate->currentText() << cbbox_scale->currentText();
}

void SystemSettings::writeCfgToFile(const QString &fname)
{
    QFile fd(fname);

    if(!fd.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::warning(this,"出错啦!","不能创建文件："+fname);
        return;
    }

    QTextStream stream(&fd);
     stream.setAutoDetectUnicode(false);
     stream.setGenerateByteOrderMark(true);
     stream.setCodec("UTF-8");

     stream << "Output=" << edt_dir->text() << "\r\n";
     QString tmp = cbox_autoopen->isChecked() ? "1" : "0";
     stream << "AutoOpen=" << tmp << "\r\n";
     UpdateCurrentIndexText();
     for(int i = 0 ; i < m_listkey.count();i++)
     {
         stream << m_listkey.at(i) << m_currentIndexText.at(i) << "\r\n";
     }

     fd.close();
}

void SystemSettings::readCfgToFile(const QString &fname)
{
    if(!QFileInfo(fname).exists())
        return;
    QFile fd(fname);
    if(!fd.open(QIODevice::ReadOnly|QIODevice::Text))
    {
        QMessageBox::warning(this,"出错啦!","不能创建文件："+fname);
        return;
    }

    QTextStream stream(&fd);
     stream.setAutoDetectUnicode(false);
     stream.setGenerateByteOrderMark(true);
     stream.setCodec("UTF-8");
     QString readline;
     m_currentIndexText.clear();
     while(!fd.atEnd())
     {
         readline = fd.readLine().trimmed();
         if(readline.isEmpty())
             continue;
         readline = readline.section('=',1,1);
         m_currentIndexText.append(readline);
     }
     fd.close();
     edt_dir->setText(m_currentIndexText.takeAt(0));
     int v = m_currentIndexText.takeAt(0).toInt();
     cbox_autoopen->setChecked(v == 1 ? true : false);

//     edt_height->setText(m_currentIndexText.takeAt(0));
//     edt_width->setText(m_currentIndexText.takeAt(0));
     for(int i = 0; i < m_listcbbox.count();i++)
     {
         m_listcbbox[i]->setCurrentText(m_currentIndexText.at(i));
     }

}

SystemSettings::~SystemSettings()
{

}


void SystemSettings::setDefaultCfg()
{

}
