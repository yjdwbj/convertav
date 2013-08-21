#include "mylistwidget.h"

MyListWidget::MyListWidget(QWidget *parent)
    :QListWidget(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::InternalMove);
//    dropHintItem = new QListWidgetItem("拖入文件放这里...",this);
    setToolTip("请拖入需要转换的视频文件");

}

MyListWidget::~MyListWidget()
{

}

void MyListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
        QListWidget::dragMoveEvent(event);
}

void MyListWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
        QListWidget::dragEnterEvent(event);
}

void MyListWidget::dropEvent(QDropEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        if(!urls.isEmpty())
        {
                if(dropHintItem)
                {
                    delete dropHintItem;
                    dropHintItem = 0;
                }

                emit hasUrls(urls);
        }
        event->acceptProposedAction();
    }
    QListWidget::dropEvent(event);
}

