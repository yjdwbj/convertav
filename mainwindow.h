#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QWidgetItem>
#include <QStandardItem>
#include <QFileDialog>
#include <QProcess>
#include <QTime>
#include <QPair>
#include "itemoflistfiles.h"
#include "previewplay.h"
#include "toolboxsettings.h"
#include "global.h"
#include "mylistwidget.h"
#include "myframe.h"
#include <QMessageBox>
#include <QAction>



class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void slot_openfiles();
    void slot_DClickToPrePlay(QModelIndex);
    void slot_DClickToPrePlay(QListWidgetItem *);
    void slot_ClickToSetCurrentRow(QListWidgetItem *);
    void slot_ConvertAll();
    void slot_Settings();
    void slot_removeItem(QWidget *);
    void slot_removeAllItem(QAction *);
    void slot_GotUrls(QList<QUrl>);
    void slot_GotFileListFromGuide(QStringList);
    void slot_ReadOutputFromScreen();
    void slot_RightClickMenu();

    ConvertCfg getConvertCfg() const {return m_ConvertCfg;}
private:
    void FilesOrDirNoExists(const QString &in);
//    friend class ItemView;
    QGridLayout* layout_main;
    QPushButton* btn_OpenFile;
    QPushButton* btn_ConvertAll;
    QPushButton* btn_Settings;
    MyListWidget* lwt_ConverFiles;
    PreViewPlay *m_PreViewPlay;
    QProcess *m_PrePlayProcess;
    QProcess *m_screenProcess;
    QString m_ffmpeg_info;
    ToolBoxSettings *m_ToolBoxSettings;
    QString m_configfile;
    QString m_mplayer;
    QString m_AppPath;
//    QString m_mencoder;
    QString m_ffmpeg;
    QList<QListWidgetItem*> m_listwidgetitem;
    QList<itemstruct> m_listitemstruct;
    QStringList m_listItems;
    bool m_AfterConvertAutoOpen;
    QPalette m_palette;
    QBrush m_brush;
    QString m_LastPath;
    MyFrame* m_guide;

    ConvertCfg m_ConvertCfg;


    void ReadOrCreateCfg();
    void SwitchMainListToGuide();
    void fillFiletoListWidget(QStringList &list);
    QStringList FFMPEGScreenshotArg(const QString &file);
    void FormatVideoInfo(const QStringList &data,itemstruct &result,const QString &fnamel);
    void FortmatVideoInfoMplayer(const QStringList &readlist, itemstruct &item);
};

#endif // MAINWINDOW_H
