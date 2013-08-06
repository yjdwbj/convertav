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
#include "previewplay.h"



class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void slot_openfiles();
    void slot_DClickToPrePlay(QModelIndex);
private:
    QGridLayout* layout_main;
    QPushButton* btn_OpenFile;
    QPushButton* btn_ConvertAll;
    QListWidget* lwt_ConverFiles;
    PreViewPlay *m_PreViewPlay;
    QProcess *m_PrePlayProcess;
    QString m_AppPath;
    QString m_mplayer;
    QString m_mencoder;
    QStringList m_listItems;

    void fillFiletoListWidget(const QStringList &list);
};

#endif // MAINWINDOW_H
