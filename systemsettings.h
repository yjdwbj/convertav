#ifndef SYSTEMSETTINGS_H
#define SYSTEMSETTINGS_H

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QGroupBox>
#include <QDesktopServices>


class SystemSettings: public QWidget
{
    Q_OBJECT
public:
    explicit SystemSettings(QWidget *parent = 0);
    ~SystemSettings();
private:
    QGridLayout* layout_main;
    QGroupBox *box_output;
    QGroupBox *box_audio;
    QGroupBox *box_video;

    QString m_outputDir;

};

#endif // SYSTEMSETTINGS_H
