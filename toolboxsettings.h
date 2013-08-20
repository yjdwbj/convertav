#ifndef TOOLBOXSETTINGS_H
#define TOOLBOXSETTINGS_H

#include <QtWidgets/QPushButton>
#include <QtWidgets/QToolBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include "systemsettings.h"
#include <QtWidgets/QTimeEdit>
#include <QTableView>
#include <QTableWidget>

class ToolBoxSettings: public QWidget
{
    Q_OBJECT
public:

    explicit ToolBoxSettings(QWidget *parent = 0);
    ~ToolBoxSettings();
    void setFilmHW(QPair<QString,QString> );
    void updateToolBox(QStringList list);
    ConvertCfg getConvertArgments() const{return m_ConvertCfg;}
    void updateStructConvertCfg();
    void setTimeAndNameToTable(const QPair<QString, QString> &pair);

private slots:

    void SomeValueHasChanged(QString);
    void slot_updateTime();
private:
    void setBaseSettings();
    void setAudioSettings();
    void setVideoSettings();
    void CreateTotalTimeLayout();
    QVBoxLayout* base_Layout;
    QVBoxLayout* audio_Layout;
    QVBoxLayout* video_Layout;
    QToolBox *main_ToolBox;
    QLineEdit *edt_fname;

    QComboBox* cbox_vencoder;
    QComboBox* cbox_hwrate;
    QComboBox* cbox_vbitrate;
    QComboBox* cbox_enoderCount;
    QComboBox* cbox_frameRate;
    QComboBox* cbbox_aencoder;
    QComboBox* cbbox_abitrate;
    QComboBox* cbbox_channel;
    QComboBox* cbbox_samplerate;
    QTableWidget *tw_time;
    QTableWidgetItem *twi_start;
    QTableWidgetItem *twi_end;
    QTableWidgetItem *twi_time;


    QTimeEdit* tedit_start;
    QTimeEdit* tedit_end;
    SystemSettings *m_sys;
    QList<QComboBox*> m_listComboBox;
    QLineEdit *edt_high;
    QLineEdit *edt_width;
    ConvertCfg m_ConvertCfg;




};

#endif // TOOLBOXSETTINGS_H
