#include "myframe.h"

MyFrame::MyFrame(QWidget *parent)
    :QFrame(parent)
{

    setAcceptDrops(true);
    setToolTip("请拖入需要转换的视频文件");

    setObjectName("Guide");
    setStyleSheet("background-color: #ffffff;");
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addSpacerItem(new QSpacerItem(100,200));

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addSpacerItem(new QSpacerItem(50,50));

    QVBoxLayout *tlay = new QVBoxLayout;// item1,item2,item3

    QLabel* lab_image = new QLabel;
    lab_image->setPixmap(QPixmap(":/lcy/image/free.png"));

    QLabel* lab_text = new QLabel("点击下面按钮或拖放添加视频");
    lab_text->setStyleSheet("color: gray;"
                            "font: bold 14px;");
    QPushButton* btn_addVideo = new QPushButton("添加文件");

    connect(btn_addVideo,SIGNAL(clicked()),SLOT(HasClickBotton()));
    QImage img(":/lcy/image/mainAddVideobtn.png");
    int h = img.height()/4;
    int w = img.width();
    btn_addVideo->setFixedSize(w,h);
    btn_addVideo->setStyleSheet("background-image: url("
                                ":/lcy/image/mainAddVideobtn.png);"
                                "background-position: top center;"
                                "border-radius: 6px;"
                                "background-origin: margin;"
                                "border-width: 2px;"
                                "font: bold 20px;"
                                "color: beige;"
                                "border-style: outset;"
                                "font-family: SimHei;");

    tlay->addWidget(lab_image,50,Qt::AlignHCenter);
    tlay->addWidget(lab_text,Qt::AlignHCenter);
    tlay->addWidget(btn_addVideo,Qt::AlignHCenter);
    hlay->addLayout(tlay);
    hlay->addSpacerItem(new QSpacerItem(50,50));
    vlay->addLayout(hlay);
    vlay->addSpacerItem(new QSpacerItem(100,200));
    setLayout(vlay);
}


MyFrame::~MyFrame()
{
    this->deleteLater();
}

void MyFrame::HasClickBotton()
{
    QFile fd(QDir::homePath()+"/Application Data/ConvertToMJPEG/lastopen");
    if(fd.open(QIODevice::ReadOnly|QIODevice::Text))
    {
//        QTextStream ts(&fd);
//        ts.setAutoDetectUnicode(false);
//        ts.setGenerateByteOrderMark(true);
//        ts.setCodec("UTF-8");
        m_LastPath =fd.readLine().trimmed();
        fd.close();
    }


    QString s_format("支持的格式 (");
    int i =0;
    while(QString(QByteArray(support_format[i])).compare("end"))
    {
        s_format.append("*.");
        s_format.append(support_format[i]);
        s_format.append(" ");
        i++;
    }
    s_format.replace(s_format.length(),1,")");
    QStringList listfiles =  QFileDialog::getOpenFileNames(this,tr("选择文件"),m_LastPath,
                                                           s_format);

    if(listfiles.count() >0)
    {
        m_LastPath = QFileInfo(listfiles.at(0)).absolutePath();

        if(fd.open(QIODevice::WriteOnly|QIODevice::Text))
        {
            QTextStream ts(&fd);
            ts.setAutoDetectUnicode(false);
            ts.setGenerateByteOrderMark(true);
            ts.setCodec("UTF-8");
            ts << m_LastPath;
            fd.close();
        }
        emit GotStringList(listfiles);
    }


}


void MyFrame::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
        QFrame::dragMoveEvent(event);
}

void MyFrame::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
        QFrame::dragEnterEvent(event);
}

void MyFrame::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        QStringList listfiles;
        int i;
        foreach(QUrl url,urls)
        {
            QString l = url.toString().remove(0,8); // remove file:///
            QString ext = QFileInfo(l).suffix();
            i = 0;

            while(QString(support_format[i]).compare("end"))
            {
                if(!ext.compare(support_format[i],Qt::CaseInsensitive))
                {
                    listfiles.append(l);
                    break;
                }
                i++;
            }
        }
        emit GotStringList(listfiles);
        event->acceptProposedAction();
    }
    QFrame::dropEvent(event);
}
