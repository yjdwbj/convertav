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
    explicit ItemView(itemstruct &item,const ToolBoxSettings *tbs, QWidget *parent = 0);
    ~ItemView();

    QPushButton* btn_convert;
    QPushButton* btn_delself;
    QProcess *m_Process;

public slots:
    void slot_destoryMySelf();
    void slot_stopConvert();
    void slot_ConvertToStandby();
    void slot_MouseOnConvert();

private slots:

    void slot_DClickToPlay();
    void slot_ConvertFinished(int);
    void slot_ConvertingStandardOutput();


private:
    QGridLayout* main_layout;

//    QLabel *lab_audio;
//    QLabel *lab_videoinfo;
    QLabel *lab_view;
//    QLabel *lab_time;
    QCheckBox *cbox_selelct;

    QString fullpath;

    itemstruct m_item;
    const ToolBoxSettings *m_ToolBoxSettings;
    QString m_mencoder;


    void setDefaultStyleSheet(QWidget *w, const QString &image);
    QLayout* CreateItemInfoLayout();
    void removeItemLayout();


Q_SIGNALS:
    void parentDeleteMe(QWidget *);
    void parentConvertSignal();







};

#endif // ITEMOFLISTFILES_H
