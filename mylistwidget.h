#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H


#include <QListWidget>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDragMoveEvent>
#include <QWheelEvent>
#include <QAction>

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget *parent=0);
    ~MyListWidget();
    QList<QUrl> getUrls() const {return m_urls;}
protected:
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
//    void mousePressEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *);

    QListWidgetItem *dropHintItem;
    QList<QUrl> m_urls;
signals:
    void hasUrls(QList<QUrl>);
    void deleteAllItems(QAction*);

};

#endif // MYLISTWIDGET_H
