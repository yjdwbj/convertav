#include "itemoflistfiles.h"
#include <QFrame>
ItemView::ItemView(itemstruct &item,QWidget *parent, Qt::WindowFlags flags)
    :QTabWidget(parent),
      main_layout(new QGridLayout),
      btn_convert(new QPushButton),
      btn_delself(new QPushButton),
      lab_audio(new QLabel),
      lab_videoinfo(new QLabel),
      cbox_selelct(new QCheckBox),
      lab_view(new QLabel("test")),
      lab_time(new QLabel)
{
    QHBoxLayout *oneline = new QHBoxLayout;
    oneline->addWidget(cbox_selelct);

    oneline->addStretch(10);
    btn_convert->setToolTip(tr("开始转换"));
    btn_convert->setFixedSize(25,19);
    btn_convert->setIconSize(btn_convert->size());
    btn_convert->setIcon(QPixmap(":/lcy/image/converter.png").copy(0,37,25,19));
    btn_convert->setObjectName("btn_convert");
//    connect(btn_convert,SIGNAL(pressed()),SLOT(slot_MouseOnConvert()));
    oneline->addWidget(btn_convert);
    btn_delself->setFixedSize(39,19);
    btn_delself->setIconSize(btn_delself->size());
    btn_delself->setIcon(QPixmap(":/lcy/image/no.png").copy(0,0,39,19));
    btn_delself->setToolTip("删除该项");
    oneline->addWidget(btn_delself);

    QHBoxLayout *twoline = new QHBoxLayout;
    lab_view->setFrameShape(QFrame::Panel);
    QImage img(item.image);

    lab_view->setFixedSize(68,54);
    lab_view->setPixmap(QPixmap::fromImage(img).scaled(68,54));
//    towline->addWidget(lab_view);
    main_layout->addWidget(lab_view,0,0,2,1);

    cbox_selelct->setCheckState(Qt::Checked);
//    towline->addWidget(cbox_selelct);

    QLabel* ico_time = new QLabel;
    setDefaultStyleSheet(ico_time,":/lcy/image/time.png");
    twoline->addWidget(ico_time);
    lab_time->setText(item.file_time);
    twoline->addWidget(lab_time);
    twoline->addStretch();

    QLabel* ico_film = new QLabel;
    setDefaultStyleSheet(ico_film,":/lcy/image/film.png");
    twoline->addWidget(ico_film);
    lab_videoinfo->setText(item.video_info);
    lab_videoinfo->setToolTip(item.video_info);
    twoline->addWidget(lab_videoinfo);


    QLabel* ico_audio = new QLabel;
    setDefaultStyleSheet(ico_audio,":/lcy/image/voice.png");
    twoline->addWidget(ico_audio);
    lab_audio->setText(item.audio_info);
    lab_audio->setToolTip(item.audio_info);
    twoline->addWidget(lab_audio);


//    main_layout->addWidget(btn_convert);

    main_layout->addLayout(oneline,0,1);
    main_layout->addLayout(twoline,1,1);
    main_layout->setSpacing(1);
    QWidget *w=new QWidget;
    w->setLayout(main_layout);
    addTab(w,item.filename);
    this->setToolTip(item.filename);


//    setTabsClosable(true);

}


void ItemView::setDefaultStyleSheet(QWidget *w, const QString &image)
{
    w->setStyleSheet("background-image: url("+image+");");
    QImage img(image);
    w->setFixedSize(img.size());
}

ItemView::~ItemView()
{

}

void ItemView::slot_DClickToPlay()
{

}


void ItemView::slot_MouseOnConvert()
{
    findChild<QPushButton*>("btn_convert")->setIcon(QPixmap(":/lcy/image/converter.png").copy(0,38,25,19));
}

