#ifndef MYFRAME_H
#define MYFRAME_H

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFile>
#include <QDir>
#include "global.h"
#include <QFileDialog>
#include <QTextStream>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QDragMoveEvent>
#include <QMimeData>

class MyFrame: public QFrame
{
    Q_OBJECT
public:
    explicit MyFrame(QWidget* parent = 0);
    ~MyFrame();
signals:
    void GotStringList(QStringList);
private slots:
    void HasClickBotton();

private:
    QString m_LastPath;
protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dragMoveEvent(QDragMoveEvent *e);
    void dropEvent(QDropEvent *e);

};

#endif // MYFRAME_H
