#ifndef MYGRAPHICSSCENE_H
#define MYGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>

class MyGraphicsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyGraphicsScene(QObject *parent = 0);

public:
    enum Type {
        T_None,
        T_Point,
        T_Line,
        T_Circle,
        T_Ellipse,
        T_Rect,
        T_RoundRect,
        T_Polygon,
        T_Bezier
    };
    enum State {
        S_None,
        S_Begin,
        S_Moving,
        S_Press,
        S_End
    };
private:
    void beginDraw(QPoint beginPos,QPoint nextPos,int type);
    void movingDraw(QPoint beginPos,QPoint movingPos,int type);
    void pressDraw(QPoint beginPos,QPoint pressPos,int type);
    void endDraw(QPoint beginPos,QPoint endgPos,int type);
    void clear(int type);
private:
    QList<QGraphicsItem*> mItemList;
    QGraphicsItem *mCurrentItem;
//    QGraphicsLineItem *lineItem;
//    QGraphicsRectItem *rectItem;

public slots:
    void onDrawImage(QPoint beginPos,QPoint nextPos,int type,int state);
    void clearLineItems();
    void clearRectItems();
};

#endif // MYGRAPHICSSCENE_H
