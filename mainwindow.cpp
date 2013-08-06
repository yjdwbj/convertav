#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemoflistfiles.h"
#include "toolboxsettings.h"
#include <QToolBar>
#include <QTabWidget>
#include "global.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    layout_main(new QGridLayout),
    btn_OpenFile(new QPushButton),
    btn_ConvertAll(new QPushButton),
    lwt_ConverFiles(new QListWidget),
    m_AppPath(qApp->applicationDirPath()),
    m_mplayer(m_AppPath+ ("/mplayer.exe")),
    m_mencoder(m_AppPath + ("/mencoder.exe")),
    m_PreViewPlay(new PreViewPlay),
    m_PrePlayProcess(new QProcess)
{
//    m_AppPath = qApp->applicationDirPath();
//    m_mplayer = m_AppPath + ("/mplayer.exe");
//    m_mencoder = m_AppPath +("/mencoder.exe");
    lwt_ConverFiles->setFixedWidth(600);
  this->setStyleSheet("QLineEdit {background-color: yellow}");
    lwt_ConverFiles->setAlternatingRowColors(true);
    lwt_ConverFiles->setResizeMode(QListView::Fixed);
    lwt_ConverFiles->setAcceptDrops(true);
    lwt_ConverFiles->setToolTip("请拖入需要转换的视频文件");

    btn_ConvertAll->setText(QStringLiteral("全部转换"));
    btn_ConvertAll->setFixedSize(112,28);

    btn_OpenFile->setText(QStringLiteral("添加视频"));
    btn_OpenFile->setFixedSize(112,28);
    btn_OpenFile->setIconSize(btn_OpenFile->size());
//    QPalette pal;
//    pal.setBrush(QPalette::NColorGroups,QPalette::ButtonText,QBrush(QPixmap(":/lcy/image/video+.png").copy(0,28,112,28)));
//    btn_OpenFile->setPalette(pal);
//    btn_OpenFile->setIcon(QPixmap(":/lcy/image/video+.png").copy(0,28,112,28));
    connect(btn_OpenFile,SIGNAL(clicked()),SLOT(slot_openfiles()));
    layout_main->addWidget(btn_OpenFile,0,0);
    layout_main->addWidget(btn_ConvertAll,0,1,Qt::AlignLeft);


    layout_main->addWidget(lwt_ConverFiles,1,0,2,2);
    layout_main->addWidget(m_PreViewPlay,1,3);
    layout_main->addWidget(new ToolBoxSettings,2,3);

    QWidget *w = new QWidget(parent);
    w->setLayout(layout_main);
    setCentralWidget(w);
}

void MainWindow::slot_openfiles()
{

    QStringList listfiles =  QFileDialog::getOpenFileNames(this,tr("选择文件"),"D:/testing-Video",tr("支持的格式 (*.avi *.mp4 *.rm *.rmvb *.mkv *.wmv *.mov *.mpeg *.data)"));

    if(listfiles.isEmpty())
        return ;
    fillFiletoListWidget(listfiles);
}

void MainWindow::fillFiletoListWidget(const QStringList &list)
{


    QStringList arglist = QString("-ss 10 -vo png -ao null -identify  -frames 1").split(QRegExp("(\\s)+"));
    QProcess *p = new QProcess;
    QTime pt(0,0,0);
    QTime nt;
    for(int i = 0; i < list.count() ; i++)
    {

        m_listItems.append(list[i]);
        p->start(m_mplayer,arglist << list.at(i));
        if (!p->waitForStarted())
            return ;
        if(!p->waitForFinished())
            return ;
        QStringList readlist = QString::fromUtf8(p->readAll().constData()).split(QRegExp("(\\r\\n)"),QString::SkipEmptyParts);

        itemstruct item;
        item.filename = QFileInfo(list[i]).completeBaseName();

        QString numimg("0000000");
        numimg.append(QString::number(i));
        while(numimg.size()>8)
            numimg.remove(0,1);
        numimg.append(".png");
        item.image = QDir::currentPath()+"/"+numimg;
        for(int i = 0;i < readlist.count();i++)
        {
            if(readlist[i].startsWith("VIDEO:"))
            {
                item.video_info=readlist[i].section(":",1);
            }
            else if(readlist[i].startsWith("AUDIO:"))
            {
                item.audio_info = readlist[i].section(":",1);
            }
            else if(readlist[i].startsWith("ID_LENGTH="))
            {
                pt.setHMS(0,0,0);
                nt.setHMS(0,0,0);
                QString t = readlist[i].section("=",1);
                nt = pt.addMSecs(qint32(t.toFloat()*1000));
                item.file_time = nt.toString(tr("hh:mm:ss.zzz"));
            }
        }
        QListWidgetItem *lwt2 = new QListWidgetItem("",lwt_ConverFiles);

        lwt2->setSizeHint(QSize(500,100));
        ItemView  *iv= new ItemView(item);
        lwt_ConverFiles->setItemWidget(lwt2,iv);
        connect(lwt_ConverFiles,SIGNAL(clicked(QModelIndex)),SLOT(slot_DClickToPrePlay(QModelIndex)));


    }
}

void MainWindow::slot_DClickToPrePlay(QModelIndex mindex)
{
//   int index = mindex.row();
//   QStringList arg;
//   arg << "-slave" << "-wid" << QString::number(m_PreViewPlay->lab_preplay->winId()) << "-really-quiet"
//       << "-loop" << "0" << m_listItems[index];
//   m_PrePlayProcess->start(m_mplayer,m_listItems[index]);
   m_PreViewPlay->PrePlayFile(m_mplayer,m_listItems[mindex.row()]);
}

MainWindow::~MainWindow()
{

}
