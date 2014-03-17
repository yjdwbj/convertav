#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDesktopServices>
#include <QDir>
#include <QSize>
#include <QUrl>
#include "global.h"
#include <QTextStream>


class SystemSettings: public QDialog
{
    Q_OBJECT
public:
    explicit SystemSettings(QWidget *parent = 0);
    ~SystemSettings();
    void setOutputDir(const QString &dir) {edt_dir->setText(dir);}
//    void setVideoSize(const QSize &x) {edt_width->setText(QString::number(x.width()));edt_height->setText(QString::number(x.height()));}
//    void setVideoSize(const int w,const int h) {edt_width->setText(QString::number(w));edt_height->setText(QString::number(h));}
    void setVideoScale(const QString &size){cbbox_scale->setCurrentText(size);}
    QStringList getlistAllItems() {return m_listAllItems;}
    QStringList getCurrentIndexText() const {return m_currentIndexText;}
    void UpdateCurrentIndexText();
    QString getOutputDir() const {return edt_dir->text();}
    void writeCfgToFile(const QString &fname);
    void readCfgToFile(const QString &fname);
    bool isConvertFinishedAutoOpen() const {return cbox_autoopen->isChecked();}
    QList<QComboBox*> getAllComboBox() const {return m_listcbbox;}
    QStringList GetSupportedScale();
  //  QPair<QString,QString> getFilmHW() const { return qMakePair(edt_height->text(),edt_width->text());}
public slots:
    void slot_openOutputDir();

private slots:
    void slot_setNewOutputDir();
private:
    QGridLayout* layout_main;
    QGroupBox *box_output;
    QGroupBox *box_audio;
    QGroupBox *box_video;
    QLineEdit* edt_dir;
//    QLineEdit* edt_width;
//    QLineEdit* edt_height;
    QCheckBox* cbox_autoopen;

    QComboBox* cbox_vencoder;
    QComboBox* cbox_hwrate;
    QComboBox* cbox_vbitrate;
    QComboBox* cbox_encoderCount;
    QComboBox* cbox_frameRate;
    QComboBox* cbbox_aencoder;
    QComboBox* cbbox_abitrate;
    QComboBox* cbbox_channel;
    QComboBox* cbbox_samplerate;
    QComboBox* cbbox_scale;

    QList<QComboBox*> m_listcbbox;
    QStringList m_listAllItems;
    QStringList m_currentIndexText;



    QString m_outputDir;
    QVBoxLayout* main_Layout;

    void setDefaultCfg();
    void InitDialog();
    void FilesOrDirNoExists(const QString &in);


};

#endif // SYSTEMSETTINGS_H
