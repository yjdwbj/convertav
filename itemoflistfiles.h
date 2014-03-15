#ifndef ITEMOFLISTFILES_H
#define ITEMOFLISTFILES_H
#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QTabWidget>
#include "global.h"
#include <QtWidgets/QListWidget>
#include <QtWidgets/QProgressBar>
#include <QProcess>
#include "toolboxsettings.h"
#include <QApplication>
#include <QBitmap>




class ItemView: public QTabWidget
{
    Q_OBJECT
public:
    enum State { Stopped = 0, Converting = 1 };
    explicit ItemView(itemstruct &item,const ToolBoxSettings *tbs, QWidget *parent = 0);
    ~ItemView();

    QPushButton* btn_convert;
    QPushButton* btn_delself;
    QString m_AppPath ;
    QString m_ffmpeg;
    QString m_mplayer;
    QString m_mencoder;
    QProcess *m_Process;
    State getState()const {return _state;}
    bool isFinished() {return _state == Stopped;}

public slots:
    void slot_destoryMySelf();
    void slot_stopConvert();
    void slot_ConvertToStandby();
    void slot_MouseOnConvert();
    void slot_MouseOnConvertFFMPEG();  // 改用ffmpeg.exe 转换.
    void slot_CancelReNameFile();

private slots:
    void slot_SingleConvert();
    void slot_ConvertFinished(int);
    void slot_ConvertingStandardOutput();
    void slot_ReNameFile();
    void slot_SureReNameFile();

    void slot_EditOutOfFoucs();



private:
    State _state;
    QGridLayout* main_layout;

//    QLabel *lab_audio;
//    QLabel *lab_videoinfo;
    QLabel *lab_view;
//    QLabel *lab_time;
    QCheckBox *cbox_selelct;

    QString fullpath;

    itemstruct m_item;
    const ToolBoxSettings *m_ToolBoxSettings;



    QPushButton* btn_rename;
    QPushButton* btn_rename_accpet;
    QPushButton* btn_rename_reject;
    QLineEdit*  edt_rename;

    QLayout* CreateReNameLayout();
    void setDefaultStyleSheet(QWidget *w, const QString &image);
    QLayout* CreateItemInfoLayout();
    QLayout* CreateConvertingLayout();
    QLayout* CreateFirstLine();
    void removeItemLayout(QLayoutItem *p);
    void removeTempFile(const QString &file);


Q_SIGNALS:
    void parentDeleteMe(QWidget *);
    void parentConvertSignal();







};

#endif // ITEMOFLISTFILES_H
