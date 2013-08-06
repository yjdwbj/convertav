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

class ItemView: public QTabWidget
{
    Q_OBJECT
public:
    explicit ItemView(itemstruct &item, QWidget *parent = 0,Qt::WindowFlags flags = 0);
    ~ItemView();
//    QLayout *getLayout() const { return main_layout;}
//    QTabWidget *getTabWidget() const {return tw_obj;}

private:
    QGridLayout* main_layout;
    QPushButton* btn_convert;
    QPushButton* btn_delself;
    QLabel *lab_audio;
    QLabel *lab_videoinfo;
    QLabel *lab_view;
    QLabel *lab_time;
    QCheckBox *cbox_selelct;

    void setDefaultStyleSheet(QWidget *w, const QString &image);
private slots:
    void slot_MouseOnConvert();
    void slot_DClickToPlay();



};

#endif // ITEMOFLISTFILES_H
