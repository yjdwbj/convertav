#include "mainwindow.h"
#include "global.h"
#include "ui_mainwindow.h"
#include "toolboxsettings.h"
#include <QToolBar>
#include <QTabWidget>
#include "systemsettings.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPalette>
#include <QDebug>
#include <QTextStream>


static const char *version=" v1.05";




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    layout_main(new QGridLayout),
    btn_OpenFile(new QPushButton(QStringLiteral("添加视频"))),
    btn_ConvertAll(new QPushButton()),
    btn_Settings(new QPushButton()),
    lwt_ConverFiles(new MyListWidget),
    m_PreViewPlay(new PreViewPlay),
    m_PrePlayProcess(new QProcess),
    m_ToolBoxSettings(new ToolBoxSettings),
    m_LastPath("")
{
    m_AppPath = qApp->applicationDirPath();
    m_mplayer=m_AppPath+("/mplayer.exe");
    m_mencoder=m_AppPath+("/mencoder.exe");
    m_ffmpeg=m_AppPath+("/ffmpeg.exe");
    setWindowTitle(QString("视频转换工具")+version);

    QPixmap pixmap("::/lcy/image/bg-sel.png");
    m_brush.setTexture(pixmap);
    m_palette.setBrush(backgroundRole(), QBrush(pixmap));
    this->setStyleSheet("QListWidget::item::selected{"
                        "background-color: beige;}");



    this->setWindowFlags(Qt::WindowMinimizeButtonHint|
                         Qt::WindowCloseButtonHint);

    m_configfile = QDir::homePath()+"/Application Data/ConvertToMJPEG/convert_to_mjeg.ini";
    if(!QFileInfo(m_configfile).exists())
    {
        QDir(QDir::root()).mkpath(QFileInfo(m_configfile).absolutePath());
    }
    if(!QFileInfo(m_mplayer).exists())
    {
        FilesOrDirNoExists(m_mplayer);
    }

    if(!QFileInfo(m_mencoder).exists())
    {
        FilesOrDirNoExists(m_mencoder);
    }

    lwt_ConverFiles->setFixedWidth(600);
    lwt_ConverFiles->setObjectName("mainList");


    connect(lwt_ConverFiles,SIGNAL(itemClicked(QListWidgetItem*)),SLOT(slot_ClickToSetCurrentRow(QListWidgetItem*)));
    connect(lwt_ConverFiles,SIGNAL(hasUrls(QList<QUrl>)),SLOT(slot_GotUrls(QList<QUrl>)));
    //    lwt_ConverFiles->setAlternatingRowColors(true);
    lwt_ConverFiles->setResizeMode(QListView::Fixed);


    btn_ConvertAll->setFixedSize(201,56);
    btn_ConvertAll->setIconSize(btn_ConvertAll->size());
    btn_ConvertAll->setIcon(QPixmap(":/lcy/image/convertall-1.png").copy(2,56,203,56));
    btn_ConvertAll->setToolTip(QStringLiteral("全部转换"));
    connect(btn_ConvertAll,SIGNAL(clicked()),SLOT(slot_ConvertAll()));


    btn_OpenFile->setFixedSize(112,26);
    btn_OpenFile->setIconSize(btn_OpenFile->size());
//    btn_OpenFile->setIcon(QPixmap(":/lcy/image/video+.png").copy(0,0,112,26));
    QPalette palette;
    palette.setBrush(QPalette::All,QPalette::Button,QBrush(QPixmap(":/lcy/image/video+.png").copy(0,0,112,26)));
    btn_OpenFile->setPalette(palette);

    connect(btn_OpenFile,SIGNAL(clicked()),SLOT(slot_openfiles()));
    layout_main->addWidget(btn_OpenFile,0,0);
    layout_main->addWidget(btn_ConvertAll,0,1,Qt::AlignLeft);

    btn_Settings->setFixedSize(106,56);
    btn_Settings->setIconSize(btn_Settings->size());
    btn_Settings->setIcon(QPixmap(":/lcy/image/preset-1.png").copy(2,56,108,56));
    btn_Settings->setToolTip(QStringLiteral("系统设置"));
    layout_main->addWidget(btn_Settings,0,3,Qt::AlignRight);
    connect(btn_Settings,SIGNAL(clicked()),SLOT(slot_Settings()));


//    layout_main->addWidget(lwt_ConverFiles,1,0,2,2);
    m_guide = new MyFrame;
    connect(m_guide,SIGNAL(GotStringList(QStringList)),SLOT(slot_GotFileListFromGuide(QStringList)));
    m_guide->setFixedWidth(600);
    layout_main->addWidget(m_guide,1,0,2,2);
    layout_main->addWidget(m_PreViewPlay,1,3);
    layout_main->addWidget(m_ToolBoxSettings,2,3);

    QWidget *w = new QWidget(parent);
    w->setLayout(layout_main);
    setCentralWidget(w);
}

void MainWindow::slot_GotFileListFromGuide(QStringList list)
{

    layout_main->removeWidget(m_guide);
//    delete m_guide;
//    m_guide = 0;
    m_guide->hide();
    layout_main->addWidget(lwt_ConverFiles,1,0,2,2);
    lwt_ConverFiles->setVisible(true);
    fillFiletoListWidget(list);
}

void MainWindow::slot_ClickToSetCurrentRow(QListWidgetItem *p)
{
    ItemView  *iv= (ItemView*)lwt_ConverFiles->itemWidget(p);
   QLineEdit *le = iv->findChild<QLineEdit*>("ReName");
   if(le)
   {
       iv->slot_CancelReNameFile();
       return;
   }
   int row = lwt_ConverFiles->row(p);
    if(p==lwt_ConverFiles->currentItem())
    {
        m_PreViewPlay->PrePlayFile(m_mplayer,m_listItems[row]);
    }
    else
    {
        lwt_ConverFiles->setCurrentItem(p);
        itemstruct t = m_listitemstruct.at(row);
        m_ToolBoxSettings->setTimeAndNameToTable(qMakePair(t.file_time,t.filename));
    }

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
    QFile fd(QFileInfo(m_configfile).absolutePath()+"/lastopen");
    if(fd.open(QIODevice::ReadOnly|QIODevice::Text))
    {
//        QTextStream ts(&fd);
//        ts.setAutoDetectUnicode(false);
//        ts.setGenerateByteOrderMark(true);
//        ts.setCodec("UTF-8");
        m_LastPath =fd.readLine().trimmed();
        fd.close();
    }

    if(m_LastPath.isEmpty())
    m_LastPath = QFileInfo(m_configfile).absolutePath();
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

    }


    if(listfiles.isEmpty())
        return ;
    slot_GotFileListFromGuide(listfiles);

}


void MainWindow::slot_GotUrls(QList<QUrl> urls)
{
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
    if(!listfiles.isEmpty())
        fillFiletoListWidget(listfiles);
}

QStringList MainWindow::FFMPEGScreenshotArg(const QString &file)
{
    QFileInfo finfo(file);
    QString png = finfo.baseName().append(".png");
    QStringList arg = QString("-ss 5 -y -f image2 -vframes:v 1 -v verbose").split(QRegExp("(\\s)+"));
    arg.prepend(file);
    arg.prepend("-i");
    arg.append(png);
    return arg;
}


void MainWindow::FormatVideoInfo(const QStringList &data,itemstruct &item)
{
    QTime nt;
    nt.setHMS(0,0,0);
    for(int nn = 0;nn < data.count();nn++)
    {

        if(data[nn].contains("Stream #0:"))
        {
            if(data[nn].contains("Audio:"))
            {
                item.audio_info = data[nn].section("Audio:",1,1);
            }
                else
            {
                item.video_info = data[nn].section("Video:",1,1);
            }

        }
        else if(data[nn].contains("Duration:"))
        {
           item.file_time = nt.toString(data[nn].section(QRegExp(":|,"),1,1));
        }
    }
}

void MainWindow::slot_ReadOutputFromScreen()
{
     m_ffmpeg_info.append(m_screenProcess->readAllStandardError());
}


void MainWindow::fillFiletoListWidget(const QStringList &list)
{


    QStringList arglist = QString("-ss 10 -vo png -ao null -identify  -frames 1").split(QRegExp("(\\s)+"));


    QTime pt(0,0,0);
    QTime nt;

    QStringList readlist;
    m_screenProcess = new QProcess;
    connect(m_screenProcess,SIGNAL(readyReadStandardError()),SLOT(slot_ReadOutputFromScreen()));
    connect(m_screenProcess,SIGNAL(readyReadStandardOutput()),SLOT(slot_ReadOutputFromScreen()));
    for(int i = 0; i < list.count() ; i++)
    {
        itemstruct item;
        arglist = FFMPEGScreenshotArg(list[i]);
        item.image = QDir::currentPath()+"/"+arglist.last();
        m_listItems.append(list[i]);
        m_screenProcess->start(m_ffmpeg,arglist,QIODevice::ReadOnly);
        if (!m_screenProcess->waitForStarted())
            return ;
        if(!m_screenProcess->waitForFinished())
            return ;

        item.isConverted = false;
        item.isStandby = false;
        item.fullpath = list[i];
        item.filename = QFileInfo(list[i]).completeBaseName();

        FormatVideoInfo(m_ffmpeg_info.split(QRegExp("(\\r\\n)"),QString::SkipEmptyParts),item);
//        for(int nn = 0;nn < readlist.count();nn++)
//        {
//            if(readlist[nn].startsWith("VIDEO:"))
//            {
//                item.video_info=readlist[nn].section(":",1);
//            }
//            else if(readlist[nn].startsWith("AUDIO:"))
//            {
//                item.audio_info = readlist[nn].section(":",1);
//            }
//            else if(readlist[nn].startsWith("ID_LENGTH="))
//            {
//                pt.setHMS(0,0,0);
//                nt.setHMS(0,0,0);
//                QString t = readlist[nn].section("=",1);
//                nt = pt.addMSecs(qint32(t.toFloat()*1000));
//                item.file_time = nt.toString(tr("hh:mm:ss.zzz"));
//            }
//        }
        if(!i)
        m_ToolBoxSettings->setTimeAndNameToTable(qMakePair(item.file_time,item.filename));

        readlist.clear();
        QListWidgetItem *lwt2 = new QListWidgetItem("",lwt_ConverFiles);
        lwt2->setSizeHint(QSize(500,100));
        m_listitemstruct.append(item);
        ItemView  *iv= new ItemView(item,m_ToolBoxSettings);
        connect(iv,SIGNAL(parentDeleteMe(QWidget*)),SLOT(slot_removeItem(QWidget*)));
        lwt_ConverFiles->setItemWidget(lwt2,iv);

    }
    lwt_ConverFiles->setCurrentRow(0);
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
            m_listitemstruct.removeAt(i);
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
    if(!lwt_ConverFiles->count())
    {
        SwitchMainListToGuide();
    }

}


void MainWindow::SwitchMainListToGuide()
{
    layout_main->removeWidget(lwt_ConverFiles);
    lwt_ConverFiles->hide();
    layout_main->addWidget(m_guide,1,0,2,2);
    m_guide->setVisible(true);
}



void MainWindow::slot_DClickToPrePlay(QModelIndex mindex)
{
    QListWidgetItem *p = lwt_ConverFiles->item(mindex.row());
    ItemView  *iv= (ItemView*)lwt_ConverFiles->itemWidget(p);
   QLineEdit *le = iv->findChild<QLineEdit*>("ReName");
   if(le)
   {
       iv->slot_CancelReNameFile();
       return;
   }
   m_PreViewPlay->PrePlayFile(m_mplayer,m_listItems[mindex.row()]);
}

void MainWindow::slot_DClickToPrePlay(QListWidgetItem *p)
{
//    ItemView  *iv= (ItemView*)lwt_ConverFiles->itemWidget(p);
    m_PreViewPlay->PrePlayFile(m_mplayer,m_listItems[lwt_ConverFiles->row(p)]);

}


void MainWindow::slot_ConvertAll()
{
    if(lwt_ConverFiles->count()==0)
        return;

    ConvertCfg cfg = m_ToolBoxSettings->getConvertArgments();
    if(!QFileInfo(cfg.OutputDir).exists())
        QDir(QDir::root()).mkpath(cfg.OutputDir);

    for(int i = 0 ; i < lwt_ConverFiles->count();i++)
    {
        ItemView *iw =(ItemView *)lwt_ConverFiles->itemWidget(lwt_ConverFiles->item(i));
        iw->slot_ConvertToStandby();
    }

    for(int i = 0 ; i < lwt_ConverFiles->count();i++)
    {
//        lwt_ConverFiles->setCurrentRow(i);
        itemstruct t = m_listitemstruct.at(i);
        m_ToolBoxSettings->setTimeAndNameToTable(qMakePair(t.file_time,t.filename));
        ItemView *iw =(ItemView *)lwt_ConverFiles->itemWidget(lwt_ConverFiles->item(i));
        iw->slot_MouseOnConvert();
        while(!iw->isFinished())
            QApplication::processEvents();
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
    if(!QFileInfo(m_configfile).exists())
        s->writeCfgToFile(m_configfile);
    else
        s->readCfgToFile(m_configfile);
    if(s->exec())
    {
        s->UpdateCurrentIndexText();
        m_ToolBoxSettings->updateToolBox(s->getCurrentIndexText());
        s->writeCfgToFile(m_configfile);
    }


}

MainWindow::~MainWindow()
{

}
