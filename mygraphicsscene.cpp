#include "mygraphicsscene.h"
#include <QDebug>

MyGraphicsScene::MyGraphicsScene(QObject *parent)
    :QGraphicsScene(parent)
    ,mCurrentItem(NULL)
{

}

void MyGraphicsScene::beginDraw(QPoint beginPos, QPoint nextPos, int type)
{
//    Q_UNUSED(nextPos);
//    mCurrentItem = new QGraphicsPathItem;
//    mCurrentItem->setFlags(QGraphicsItem::ItemIsSelectable|
//                            QGraphicsItem::ItemIsFocusable);
//    mCurrentItem->setPen(QPen(Qt::red,2));

    switch(type)
    {
    case T_Point:
    case T_Line:
        mCurrentItem = new QGraphicsLineItem;
        mCurrentItem->setFlags(QGraphicsItem::ItemIsSelectable|
                               QGraphicsItem::ItemIsFocusable);
        break;
    case T_Rect:
        mCurrentItem = new QGraphicsRectItem;
        mCurrentItem->setFlags(QGraphicsItem::ItemIsSelectable|
                               QGraphicsItem::ItemIsFocusable);
    default:break;
    }
//    addItem(mCurrentItem);
}

void MyGraphicsScene::movingDraw(QPoint beginPos,QPoint movingPos,int type)
{

}
void MyGraphicsScene::pressDraw(QPoint beginPos,QPoint pressPos,int type)
{

}
void MyGraphicsScene::endDraw(QPoint beginPos,QPoint endPos,int type)
{
    qreal w = endPos.x() - beginPos.x();
    qreal h = endPos.y() - beginPos.y();
    switch(type)
    {
    case T_Point:
    {
        //CGraphicsItem *item = new CGraphicsItem(beginPos,m_currentItem);
        //Q_UNUSED(item);
    }
        break;
    case T_Line:
    {
//        QPainterPath path;
//        path.moveTo(beginPos);
//        path.lineTo(endPos);
//        mCurrentItem->setPath(path);

        mCurrentItem = this->addLine(QLine(beginPos,endPos),QPen(Qt::red,2));
    }
        break;
    case T_Rect:
    {
//        QPainterPath path;
//        path.addRect(beginPos.x(),beginPos.y(),w,h);
//        mCurrentItem->setPath(path);
        mCurrentItem = this->addRect(QRect(beginPos,endPos),QPen(Qt::red,2));
    }
        break;
    default:
        break;
    }
//    mCurrentItem->setSelected(true);
//    mCurrentItem->setFlags(QGraphicsItem::ItemIsSelectable);
    mItemList.push_back(mCurrentItem);
}

void MyGraphicsScene::onDrawImage(QPoint beginPos,QPoint nextPos,int type,int state)
{
    if(state == S_Begin){
        beginDraw(beginPos,nextPos,type);
    }
    else if(state == S_Moving){
        movingDraw(beginPos,nextPos,type);
    }
    else if(state == S_Press) {
        pressDraw(beginPos,nextPos,type);
    }
    else if(state == S_End){
        endDraw(beginPos,nextPos,type);
    }
}

void MyGraphicsScene::clear(int type)
{
//    for(auto i = 0; i < mItemList.size();i++){
//        if(type == 2 && mItemList[i]->type() == 6){
//            qDebug() << "clear lines";
//            qDebug() << "mItemList size:" << mItemList.size();
//            this->removeItem(mItemList[i]);
//        }
//        else if(type == 5 && mItemList[i]->type() == 3){
//            this->removeItem(mItemList[i]);
//        }
//    }
}

void MyGraphicsScene::clearLineItems()
{
    clear(T_Line);
}

void MyGraphicsScene::clearRectItems()
{
    clear(T_Rect);
}
