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


class ToolBoxSettings: public QWidget
{
    Q_OBJECT
public:

    explicit ToolBoxSettings(QWidget *parent = 0);
    ~ToolBoxSettings();
private:
    void setBaseSettings();
    void setAudioSettings();
    void setVideoSettings();
    QVBoxLayout* base_Layout;
    QVBoxLayout* audio_Layout;
    QVBoxLayout* video_Layout;
    QToolBox *main_ToolBox;

};

#endif // TOOLBOXSETTINGS_H
