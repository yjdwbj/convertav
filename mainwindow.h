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
#include "previewplay.h"
#include "toolboxsettings.h"
#include "global.h"
#include <QMessageBox>



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

    ConvertCfg getConvertCfg() const {return m_ConvertCfg;}
private:
    void FilesOrDirNoExists(const QString &in);
    QGridLayout* layout_main;
    QPushButton* btn_OpenFile;
    QPushButton* btn_ConvertAll;
    QPushButton* btn_Settings;
    QListWidget* lwt_ConverFiles;
    PreViewPlay *m_PreViewPlay;
    QProcess *m_PrePlayProcess;
    ToolBoxSettings *m_ToolBoxSettings;
    QString m_configfile;
    QString m_AppPath;
    QString m_mplayer;
    QString m_mencoder;
    QList<QListWidgetItem*> m_listwidgetitem;
    QList<itemstruct> m_listitemstruct;
    QStringList m_listItems;
    bool m_AfterConvertAutoOpen;
    QPalette m_palette;
    QBrush m_brush;
    QString m_LastPath;

    ConvertCfg m_ConvertCfg;

    void ReadOrCreateCfg();
    void fillFiletoListWidget(const QStringList &list);
};

#endif // MAINWINDOW_H
