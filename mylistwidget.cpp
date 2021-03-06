#include "mylistwidget.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>

const QString menuActions="删除|全部删除|-|开始所有转换...|打开源文件目录...";

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


void MyListWidget::contextMenuEvent(QContextMenuEvent *)
{
    // event->accept();

        QMenu* menu = new QMenu();
        menu->addAction("全部删除");
        connect(menu,SIGNAL(triggered(QAction*)),SIGNAL(deleteAllItems(QAction*)));
//        QStringList llist = menuActions.split("|");
//        for(int i = 0 ; i < llist.size();i++)
//        {
//            if(!llist[i].compare("-"))
//            {
//                menu->addSeparator();
//            }
//            else
//            {
//                menu->addAction(llist[i]);
//            }
//        }

//        if(this->count() == 0)
//            menu->setEnabled(false);

        menu->exec(QCursor::pos());

       delete menu;
}
