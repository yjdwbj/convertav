#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "itemoflistfiles.h"
#include "toolboxsettings.h"
#include <QToolBar>
#include <QTabWidget>
#include "global.h"
#include "systemsettings.h"
#include <QKeyEvent>
#include <QMouseEvent>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    layout_main(new QGridLayout),
    btn_OpenFile(new QPushButton(QStringLiteral("添加视频"))),
    btn_ConvertAll(new QPushButton()),
    btn_Settings(new QPushButton()),
    lwt_ConverFiles(new QListWidget),
    m_AppPath(qApp->applicationDirPath()),
    m_mplayer(m_AppPath+ ("/mplayer.exe")),
    m_mencoder(m_AppPath + ("/mencoder.exe")),
    m_PreViewPlay(new PreViewPlay),
    m_PrePlayProcess(new QProcess),
    m_ToolBoxSettings(new ToolBoxSettings)
{
//    m_AppPath = qApp->applicationDirPath();
//    m_mplayer = m_AppPath + ("/mplayer.exe");
//    m_mencoder = m_AppPath +("/mencoder.exe");

    if(!QFileInfo(m_mplayer).exists())
    {
        FilesOrDirNoExists(m_mplayer);
    }

    if(!QFileInfo(m_mencoder).exists())
    {
        FilesOrDirNoExists(m_mencoder);
    }

    lwt_ConverFiles->setFixedWidth(600);

//    this->setStyleSheet("QLineEdit {background-color: yellow}");
    connect(lwt_ConverFiles,SIGNAL(clicked(QModelIndex)),SLOT(slot_DClickToPrePlay(QModelIndex)));
//    connect(lwt_ConverFiles,SIGNAL(itemDoubleClicked(QListWidgetItem*)),SLOT(slot_DClickToPrePlay(QListWidgetItem*)));
    //    lwt_ConverFiles->setAlternatingRowColors(true);
    lwt_ConverFiles->setResizeMode(QListView::Fixed);
    lwt_ConverFiles->setAcceptDrops(true);
    lwt_ConverFiles->setToolTip("请拖入需要转换的视频文件");

    btn_ConvertAll->setFixedSize(201,56);
    btn_ConvertAll->setIconSize(btn_ConvertAll->size());
    btn_ConvertAll->setIcon(QPixmap(":/lcy/image/convertall-1.png").copy(2,56,203,56));
    btn_ConvertAll->setToolTip(QStringLiteral("全部转换"));
    connect(btn_ConvertAll,SIGNAL(clicked()),SLOT(slot_ConvertAll()));


    btn_OpenFile->setFixedSize(112,28);
    btn_OpenFile->setIconSize(btn_OpenFile->size());

    connect(btn_OpenFile,SIGNAL(clicked()),SLOT(slot_openfiles()));
    layout_main->addWidget(btn_OpenFile,0,0);
    layout_main->addWidget(btn_ConvertAll,0,1,Qt::AlignLeft);

    btn_Settings->setFixedSize(106,56);
    btn_Settings->setIconSize(btn_Settings->size());
    btn_Settings->setIcon(QPixmap(":/lcy/image/preset-1.png").copy(2,56,108,56));
    btn_Settings->setToolTip(QStringLiteral("系统设置"));
    layout_main->addWidget(btn_Settings,0,3,Qt::AlignRight);
    connect(btn_Settings,SIGNAL(clicked()),SLOT(slot_Settings()));


    layout_main->addWidget(lwt_ConverFiles,1,0,2,2);
    layout_main->addWidget(m_PreViewPlay,1,3);
    layout_main->addWidget(m_ToolBoxSettings,2,3);

    QWidget *w = new QWidget(parent);
    w->setLayout(layout_main);
    setCentralWidget(w);
}

void MainWindow::FilesOrDirNoExists(const QString &in)
{
    QMessageBox::warning(this,"出错啦!","找不到这个文件："+in);
}

void MainWindow::ReadOrCreateCfg()
{

    QString cfgdir = QDir::homePath()+"/ConvertAVtoMJPEG";
     QDir dir(cfgdir);
    if(!QFileInfo(cfgdir).exists())
        dir.mkpath(cfgdir);
    QString outputdir = QDir::homePath()+"/My Documents/ConvertToMJPEG";
    if(!QFileInfo(outputdir).exists())
       dir.mkpath(outputdir);
}

void MainWindow::slot_openfiles()
{

    QStringList listfiles =  QFileDialog::getOpenFileNames(this,tr("选择文件"),"D:/testing-Video",tr("支持的格式 (*.avi *.mp4 *.rm *.rmvb *.mkv)"));

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
    QStringList readlist;
    for(int i = 0; i < list.count() ; i++)
    {

        m_listItems.append(list[i]);
        p->start(m_mplayer,arglist << list.at(i));
        if (!p->waitForStarted())
            return ;
        if(!p->waitForFinished())
            return ;
         readlist = QString::fromUtf8(p->readAll().constData()).split(QRegExp("(\\r\\n)"),QString::SkipEmptyParts);
        itemstruct item;
        item.isConverted = false;
        item.isStandby = false;
        item.fullpath = list[i];
        item.filename = QFileInfo(list[i]).completeBaseName();

        QString numimg("0000000");
        numimg.append(QString::number(i));
        while(numimg.size()>8)
            numimg.remove(0,1);
        numimg.append(".png");
        item.image = QDir::currentPath()+"/"+numimg;

//        foreach(const QString &line,readlist)
//        {
//            if(line.startsWith("VIDEO:"))
//                item.video_info = line.section(":",1);
//            else if(line.startsWith("AUDIO:"))
//                item.audio_info = line.section(":",1);
//            else if(line.startsWith("ID_LENGTH="))
//            {
//                pt.setHMS(0,0,0);
//                nt.setHMS(0,0,0);
//                QString t = line.section("=",1);
//                nt = pt.addMSecs(qint32(t.toFloat()*1000));
//                item.file_time = nt.toString(tr("hh:mm:ss.zzz"));
//            }
//        }

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
        readlist.clear();
        QListWidgetItem *lwt2 = new QListWidgetItem("",lwt_ConverFiles);
        lwt2->setSizeHint(QSize(500,100));
        ItemView  *iv= new ItemView(item,m_ToolBoxSettings);
        connect(iv,SIGNAL(parentDeleteMe(QWidget*)),SLOT(slot_removeItem(QWidget*)));
        lwt_ConverFiles->setItemWidget(lwt2,iv);

    }
}
void MainWindow::slot_removeItem(QWidget *p)
{
    for(int i = 0; i < lwt_ConverFiles->count();i++)
    {
        QListWidgetItem *tmp =  lwt_ConverFiles->item(i);
        if(p == lwt_ConverFiles->itemWidget(tmp))
        {
            lwt_ConverFiles->removeItemWidget(tmp);
            lwt_ConverFiles->takeItem(i);
            m_listItems.removeAt(i);
            m_PreViewPlay->slot_Stop_Clicked();
//            QMouseEvent qm2(QEvent::MouseButtonPress, m_PreViewPlay->btn_stop->pos(), Qt::LeftButton , Qt::LeftButton,    Qt::NoModifier);
//            QApplication::sendEvent(m_PreViewPlay->btn_stop,
//                                    new QMouseEvent(QEvent::MouseButtonPress,
//                                                    m_PreViewPlay->btn_stop->pos(),
//                                                    Qt::LeftButton ,
//                                                    Qt::LeftButton,Qt::NoModifier));
//            QApplication::sendEvent(m_PreViewPlay->btn_stop,
//                                    new QMouseEvent(QEvent::MouseButtonRelease,
//                                                    m_PreViewPlay->btn_stop->pos(),
//                                                    Qt::LeftButton ,
//                                                    Qt::LeftButton,Qt::NoModifier));
            break;
        }
    }

}




void MainWindow::slot_DClickToPrePlay(QModelIndex mindex)
{
   m_PreViewPlay->PrePlayFile(m_mplayer,m_listItems[mindex.row()]);
}


void MainWindow::slot_DClickToPrePlay(QListWidgetItem *p)
{
    m_PreViewPlay->PrePlayFile(m_mplayer,m_listItems[lwt_ConverFiles->row(p)]);
}

void MainWindow::slot_ConvertAll()
{
    if(lwt_ConverFiles->count()==0)
        return;

    for(int i = 0 ; i < lwt_ConverFiles->count();i++)
    {
        ItemView *iw =(ItemView *)lwt_ConverFiles->itemWidget(lwt_ConverFiles->item(i));
        iw->slot_ConvertToStandby();
    }
    for(int i = 0 ; i < lwt_ConverFiles->count();i++)
    {
        ItemView *iw =(ItemView *)lwt_ConverFiles->itemWidget(lwt_ConverFiles->item(i));
        iw->slot_MouseOnConvert();
        while(iw->m_Process->waitForFinished(2000))
            QCoreApplication::processEvents();

    }
    SystemSettings *s = new SystemSettings;
    if(s->isConvertFinishedAutoOpen())
        s->slot_openOutputDir();



}

void MainWindow::slot_Settings()
{
    SystemSettings *s = new SystemSettings;
    QFile qss(":/lcy/image/systemsetting.qss");
    if(!qss.open(QIODevice::ReadOnly|QIODevice::Text))
        return;
    s->setStyleSheet(qss.readAll());
    qss.close();
    if(s->exec())
    {
        m_ToolBoxSettings->updateToolBox(s->getCurrentIndexText());
    }

}

MainWindow::~MainWindow()
{

}
